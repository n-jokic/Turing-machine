#include "command.h"
#include "dictionary.h"
#include <ctype.h>

HATS generateCommandList(char* filename, char* currentDictionary) {
	OState* tempState;
	OState tempStateData;
	int charPos = 0;
	char tempChar = 'b';
	char c[2] = "r";
	const char* chr = &c;
	FILE* file;
	HATS tempHats = { NULL, NULL, 0 };

	tempState = initTempState();
	if (!tempState) {
		return (HATS) { NULL, NULL, 1 };
	}
	
	file = fopen(filename, chr);
	if (!file) {
		freeOState(tempState);
		return (HATS) { NULL, NULL, 1 };
	}

	while ((tempChar!= EOF )&&( (tempChar = getc(file)) != EOF)) { //Da li je EOL preskacE?
		if (tempChar == '\n') continue;
		charPos++;
		tempStateData = readCommand(file, &tempChar, currentDictionary, &charPos);
		if (tempStateData.error) {
			fclose(file);
			freeOState(tempState);
			deleteCommandList(tempHats);
			return (HATS) { NULL, NULL, 1 };
		}

		*(tempState) = tempStateData;
		tempHats = formList(&tempHats, tempState); //Ulancavanje
		if (tempHats.error) {
			fclose(file);
			freeOState(tempState);
			deleteCommandList(tempHats);
			return (HATS) { NULL, NULL, 1 };
		}

		tempState = initTempState();
		if (!tempState) {
			fclose(file);
			deleteCommandList(tempHats);
			return (HATS) { NULL, NULL, 1 };
		}
	}

	fclose(file);
	freeOState(tempState);
	return tempHats;
}

int linkCommandList(HATS tempHats) { // U main() {...}
	OState* current = tempHats.head;

	while (current) {
		if (linkNStates(current, tempHats.head)) { //greska == 1
			return 1;
		}
		current = current->next;
	}
	return 0;
}
int linkNStates(OState* current, OState* head) {
	int i = current->numOfNewStates;

	for (int j = 0; j < i; j++) {
		if (searchList(*(current->newStates + j), head)) { //greska ==1
			return 1;
		}
	}
	return 0;
}

int searchList(NState* currentN, OState* head) {
	OState *temp = head;
	if (currentN->newState == -1 || currentN->newState == -2) return 0; //-1 je oznaka potvrdan odvovor, a -2 je oznaka za negativan odgovor
	while ((currentN->newState != temp ->oldState) && (currentN->newState > temp ->oldState) && temp && temp->next) {
		temp = temp->next;
	}
	if (currentN->newState != temp->oldState) {
		printf("Greska nije definisan sledeci korak za stanje q%d", currentN->newState);
		return 1;
	}
	currentN->nextState = temp;
	return 0;
}
HATS formList(HATS *tempHats, OState* tempState) {
	OState* head = (*tempHats).head;
	OState* tail = (*tempHats).tail;
	if (!head) {
		head = tempState;
		tail = tempState;
		return (HATS) { head, tail, 0 };
	}

	if (findPlacement(&head, &tail, &tempState)) {
		(*tempHats).error = 1;
		return *tempHats;
	}
	tempHats->head = head;
	tempHats ->tail = tail;
	return *tempHats;
}

int findPlacement(OState** head, OState** tail, OState** tempState) {

	if ((*tempState)->oldState > (*tail)->oldState) {
		addToEnd(tail, *tempState);
		return 0;
	}
	if ((*tempState)->oldState < (*head)->oldState) {
		addToBeg(head, *tempState);
		return 0;
	}

	if (addToMid(*head, tempState)) { //ako je greska vraca 1
		return 1;
	}
	return 0;
}
int addToMid(OState *head, OState** tempState) {
	OState* current = head;

	while (current && current->next) {
		if ((*tempState)->oldState > current->oldState && (*tempState)->oldState < current->next->oldState) {
			insertMid(current, *tempState);
			return 0;
		}
		if ((*tempState)->oldState == current->oldState) {
			if (replaceMid(current, tempState)) return 1;
			return 0;
		}
		current = current->next;
	}
	if(current->oldState==(*tempState)->oldState) 
		if (replaceMid(current, tempState)) return 1;
		else return 0;
	return 1;
}

int replaceMid(OState* current, OState** tempState) {
	NState** temp;
	current->numOfNewStates++;

	temp = (NState**)realloc(current->newStates, sizeof(NState*)*current->numOfNewStates);
	if (!temp) {
		return 1;
	}

	current->newStates = temp;
	*(current->newStates + current->numOfNewStates - 1) = *((*tempState)->newStates);
	free(*tempState);
	return 0;
}
void insertMid(OState* current, OState* tempState) {
	OState* temp;
	temp = current->next;
	temp->prev = tempState;
	current->next = tempState;
	tempState->prev = current;
	tempState->next = temp;
}
void addToBeg(OState** head, OState* tempState) {
	(*head)->prev = tempState;
	tempState->next = *head;
	tempState->prev = NULL;
	(*head) = tempState;
}
void addToEnd(OState** tail, OState* tempState) {
	(*tail)->next = tempState;
	tempState->prev = *tail;
	tempState->next = NULL;
	*tail = tempState;
}
HATS initTempHats(OState tempState) {
	OState* head = NULL;
	OState* tail = NULL;
	
	head = &tempState;
	tail = &tempState;
	head->next = NULL;
	head->prev = NULL; 
	return (HATS) { head, tail, 0 };
}

void deleteCommandList(HATS tempHats) {
	OState* old = tempHats.head;
	OState* next = tempHats.head;

	while (next && next->next) {
		old = next;
		if(!(next->next)) next = next->next;
		freeOState(old);
	}
	freeOState(next);
}
OState readCommand(FILE* file, char* tempChar, char* currentDictionary, int* charPos) {
	int flag = 0, oldState = 0, newState = 0, i = 0, move = 0, negative = 0;
	char oldChar, newChar;
	NState* tempNewState = NULL;
	OState* tempOldState = NULL;

	do {
		if (*tempChar == '\n') continue;
		switch (flag) {
		case 0:  //Nailazak na levu zagradu
			i = 1;
			if (*tempChar != LEFT_PARENTH) {
				printf("Na poziciji %d fali %c \n", *charPos, LEFT_PARENTH);
				return (OState) { 0, 0, NULL, NULL, NULL, 1 };
			}

			flag = 1;
			break;
			////////////////////////////////////////////
		case 1:
			if (!isalpha(*tempChar)) { // Nailazak na oznaku stanja
				printf("Na poziciji %d se ocekuje karakter", *charPos);
				return (OState) { 0, 0, NULL, NULL, NULL, 1 };
			}
			flag = 2;
			break;

		case 2: //Citanje vrednosti stanja
			if (isnum(*tempChar)) {
				oldState = oldState * i + *tempChar - '0';
				i *= 10;
			}
			else { // Nailazak na zarez
				if (*tempChar == SEPARATOR) {
					flag = 3;
				}
				else
				{
					printf("Na poziciji %d fali %c \n", *charPos, SEPARATOR);
					return (OState) { 0, 0, NULL, NULL, NULL, 1 };
				}
			}
			break;
			////////////////////////////////////////////
		case 3:
			//Ocekuje se slovo iz recnika nakon zareza
				oldChar = *tempChar;
				if (!searchDictionary(oldChar, currentDictionary)) {
					printf("%c ne postoji u recniku, pozicija %d \n", oldChar, *charPos);
					return (OState) { 0, 0, NULL, NULL, NULL, 1 };// greska
				}
				flag = 4;
			
			
				break;
				////////////////////////////////////////////
		case 4:
			if (!(*tempChar == RIGHT_PARENTH)) {// Nailazak na desnu zagradu
				printf("Na poziciji %d se ocekuje %c", *charPos, RIGHT_PARENTH);
				return (OState) { 0, 0, NULL, NULL, NULL, 1 };
			}
			flag = 5;
			break;
			////////////////////////////////////////////
		case 5:
			if (!(*tempChar == CONNECTOR)) { //Nailazak na karakter koji povezuje komande
				printf("Na poziciji %d se ocekuje %c", *charPos, CONNECTOR);
				return (OState) { 0, 0, NULL, NULL, NULL, 1 };
			}
			flag = 6;
			break;
		case 6:
			i = 1; // Restartovanje brojaca
			if (*tempChar != LEFT_PARENTH) { //Nailazak na levu zagradu
				printf("Na poziciji %d fali %c \n", *charPos, LEFT_PARENTH);
				return (OState) { 0, 0, NULL, NULL, NULL, 1 };
			}

			flag = 7;
			break;
			////////////////////////////////////////////
		case 7:
			if (!isalpha(*tempChar)) { //Nailazak na oznaku stanja
				printf("Na poziciji %d se ocekuje karakter", *charPos);
				return (OState) { 0, 0, NULL, NULL, NULL, 1 };
			}
			flag = 8;
			break;
			////////////////////////////////////////////
		case 8: //Citanje vrednosti stanja
			if (newState == 0 && *tempChar == '+') {
				newState = -1;
				break;
			}
			if (newState == 0 && *tempChar == '-') {
				newState = -2;
				break;
			}
			if (isnum(*tempChar) && newState>=0) {
				newState = newState * i + *tempChar - '0';
				i *= 10;
			}
			else { // Nailazak na zarez
				if (*tempChar == SEPARATOR) {
					flag = 9;
				}
				else {
					printf("Na poziciji %d fali %c \n", *charPos, SEPARATOR);
					return (OState) { 0, 0, NULL, NULL, NULL, 1 };
				}
			}
			break;
			////////////////////////////////////////////
		case 9:
			 //Ocekuje se slovo iz recnika nakon zareza
				newChar = *tempChar;
				if (!searchDictionary(newChar, currentDictionary)) {
					printf("%c ne postoji u recniku, pozicija %d \n", newChar, *charPos);
					return (OState) { 0, 0, NULL, NULL, NULL, 1 };
				}
				flag = 10;
			
			
			break;
			////////////////////////////////////////////
		case 10: //Nailazak na zarez
			i = 1; // Restartovanje brojaca za sledeci korak
			if (*tempChar == SEPARATOR) {
				flag = 11;
				break;
			}
			else {
				printf("Na poziciji %d fali %c \n", *charPos, SEPARATOR);
				return (OState) { 0, 0, NULL, NULL, NULL, 1 };
			}
			////////////////////////////////////////////
		case 11: //Citanje pomeraja
			if (*tempChar == '-') {
				negative = 1;
				break;
			}
			if (isnum(*tempChar)) {
				move = move * i + *tempChar - '0';
				i *= 10;
			}
			else { // Nailazak desnu zagradu
				if (*tempChar == RIGHT_PARENTH) {
					if (negative) {
						move = move * (-1);
					}
					flag = 12;
				}
				else {
					printf("Na poziciji %d fali %c \n", *charPos, RIGHT_PARENTH);
					return (OState) { 0, 0, NULL, NULL, NULL, 1 };
				}
			}
			break;
			///////////////////////////////////////////
		case 12: //Priprema za ponovo citanje
			if (*tempChar == SEPARATOR) {
				flag = 13;
			}
			else {
				printf("Na poziciji %d fali %c \n", *charPos, SEPARATOR);
				return (OState) { 0, 0, NULL, NULL, NULL, 1 };
			}
			break;
			}

			if (flag == 13) break;
			(*charPos)++;
		} while (( *tempChar != EOF) && ((*tempChar = getc(file)) != EOF));

		if (flag == 13 || (*tempChar == EOF)) {
			tempOldState = initTempOldState(oldState, newState, oldChar, newChar, move);
			if (!tempOldState) {
				return (OState) { 0, 0, NULL, NULL, NULL, 1 };
			}
			return *tempOldState;
		}
		else { //Nijeregularno ponasanje
			return (OState) { 0, 0, NULL, NULL, NULL, 1 };
		}
	}


OState* initTempOldState(int oldState, int newState, char oldChar, char newChar, int move) {
	OState* tempOldState = NULL;
	NState* tempNewState = NULL;

	tempOldState = initTempState();
	if (!tempOldState) {
		return (OState*)  {NULL};
	}

	tempNewState = initTempNState();
	if (!tempNewState) {
		freeOState(tempOldState);
		return (OState*) { NULL };
	}

	*(tempOldState->newStates) = tempNewState;
	tempNewState->oldState = tempOldState;
	tempOldState->oldState = oldState;

	tempNewState->newState = newState;
	tempNewState->oldChar = oldChar;
	tempNewState->newChar = newChar;
	tempNewState->move = move;

	return tempOldState;
}

void freeOState(OState* tempOState) {
	if (!tempOState->newStates) {
		for (int i = 0; i < tempOState->numOfNewStates; i++) {
			free(*(tempOState->newStates + i));
		}
	}
	free(tempOState);
}

NState* initTempNState() {
	NState* tempNewState = (NState*)malloc(sizeof(NState));
	if (!tempNewState) {
		return (NState*) { NULL };
	}

	tempNewState->oldState = NULL;
	tempNewState->nextState = NULL;
	tempNewState->error = 0;
	tempNewState->move = 0;
	tempNewState->newChar = 'b';
	tempNewState->oldChar = 'b';

	return tempNewState;
}
OState* initTempState() {
	OState* tempState = (OState*)malloc(sizeof(OState));
	if (!tempState) {
		return (OState*) {NULL};
	}

	tempState->newStates = (NState**)malloc(sizeof(NState*));
	if (!tempState->newStates) {
		free(tempState);
		return (OState*) { NULL };
	}

	*(tempState->newStates) = (NState*)malloc(sizeof(NState));
	if (!*(tempState->newStates)) {
		free(tempState->newStates);
		free(tempState);
		return (OState*) { NULL };
	}

	tempState->oldState = 0;
	tempState->error = 0;
	tempState->next = NULL;
	tempState->prev = NULL;
	tempState->numOfNewStates = 1;

	return tempState;
}

int isnum(char chr) {
	int i = chr - '0';
	if (i >= 0 && i <= 9) {
		return 1;
	}
	else return 0;
}