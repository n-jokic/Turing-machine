#include "tape.h"
#include "dictionary.h"
#define BLANK 'b'

TapeHAT readTape(char* filename, char* currentDictionary) {
	FILE* file;
	char tempChr = 'b';
	char c[2] = "r";
	const char* chr = c;
	TapeHAT tempHat = (TapeHAT){NULL, NULL, NULL, 0};
	Tape* tempTape;
	int i = 0;
	
	file = fopen(filename, "r");
	if (!file) {
		return (TapeHAT) { NULL, NULL, NULL, 1 };
	}
	
	
	tempTape = initTempTape();
	if (!tempTape) {
		fclose(file);
		return (TapeHAT) { NULL, NULL, NULL, 1 };
	}

	while (i++,( tempChr != EOF )&& ((tempChr = getc(file)) != EOF)) {
		if (searchDictionary(tempChr, currentDictionary)) {
			tempTape->chr = tempChr;
			tempHat = addToTape(tempHat, tempTape);
		}
		else {
			printf("Greska na poziciji %d se nalazi %c, to slovo ne postoji u recniku", i, tempChr);
			fclose(file);
			deleteTapeList(tempHat);
			free(tempTape);
			return (TapeHAT) { NULL, NULL, NULL, 1 };
		}

		tempTape = initTempTape();
		if (!tempTape) {
			deleteTapeList(tempHat);
			fclose(file);
			return (TapeHAT) { NULL, NULL, NULL, 1 };
		}
	}

	fclose(file);
	tempHat.readPos = tempHat.head;
	return tempHat;
}

TapeHAT addToTape(TapeHAT tempHat, Tape* tempTape) {
	Tape* head = tempHat.head, *tail = tempHat.tail;
	if (!head) {
		head = tempTape;
		tail = tempTape;
	}
	else {
		tail->next = tempTape;
		tempTape->prev = tail;
		tail = tempTape;
		tail->next = NULL;
	}

	return (TapeHAT) { head, tail, head, 0 };
}

void deleteTapeList(TapeHAT tempHat) {
	Tape* temp = tempHat.head, *old = temp;

	while (temp && temp->next) {
		old = temp;
		if(temp->next) temp=temp->next;
		free(old);
	}
	free(temp);
}
Tape* initTempTape() {
	Tape* tempTape = (Tape*)malloc(sizeof(Tape));

	if (!tempTape) {
		return NULL;
	}

	tempTape->chr = BLANK;
	tempTape->next = NULL;
	tempTape->prev = NULL;
	return tempTape;
}

TapeHAT setPosition(TapeHAT tempHat, int position) {
	Tape* readPos = tempHat.readPos, *tempTape;

	if (position > 0) {
		for (int i = 0; i < position; i++) {
			if (readPos->next) readPos = readPos->next;
			else {
				tempTape = initTempTape();
				if (!tempTape) {
					return (TapeHAT) { tempHat.head, tempHat.tail, NULL, 1 };
				}

				tempHat = addToTape(tempHat, tempTape);
				readPos = readPos->next;
			}
		}
	}
	if (position < 0) {
		position *= -1;
		for (int i = 0; i < position; i++) {
			if (readPos->prev) readPos = readPos->prev;
			else {
				tempTape = initTempTape();
				if (!tempTape) {
					return (TapeHAT) { tempHat.head, tempHat.tail, NULL, 1 };
				}

				tempHat.head->prev = tempTape;
				tempHat.head = tempTape;
				readPos = readPos->prev;
			}
		}
	}

	return (TapeHAT) { tempHat.head, tempHat.tail, readPos, 0 };
}
char tapeReader(TapeHAT tempHat) {
	return tempHat.readPos->chr;
}


void printTape(TapeHAT tempHat) {
	Tape* head = tempHat.head;

	while ((head->next)) {
		printf("%c", head->chr);
		head = head->next;
	}

}