#include "command.h"
#include "dictionary.h"
#include "commandExecution.h"
#include "commandReader.h"
#include "dictionary.h"
#include "currentstate.h"
#include "tape.h"
#include "readFile.h"
#define _CRT_SECURE_NO_WARNINGS


int main() {
	DefDict* defaultDictionary = NULL;
	HATS headAndTailState = (HATS) { NULL, NULL, 0 };
	char chr, flush;
	char* filename = NULL, *currentDictionary = NULL;
	int end = 1;
	int i = 0 , j= 0; 
	int user = 0;;
	TapeHAT tapeHeadAndTail = (TapeHAT) { NULL, NULL, NULL, 0 };
	HATS programHeadAndTail = (HATS) { NULL, NULL, 0 };
	State internalState;
	NState* programInput = NULL;
	OState* programTracker = NULL;
	Dict userDictionary;

	
	defaultDictionary = initDefaultDict();
	if (!defaultDictionary) {
		fprintf(stderr, "\ndefault dict error\n");
	}


	while (end) {
		if(j!=0)flush = getchar();
		printf("Da li zelite da koristite ugradjenje recnike? (Y/N):  ");
		scanf("%c", &chr);
		flush = getchar();
		j++;

		if ((chr != 'Y' && chr != 'N' && (chr != 'y') && (chr != 'n'))) { //Odabir recnika
			fprintf(stderr, "\nUnexpected input\n");
			printf("Unesite 0 za kraj programa: ");
			scanf("%d", &end);
			continue;
		}

		if (chr == 'Y' || chr == 'y') {
			i = 0;
			printf("Izaberite recnik: \n\n");
			printf("Recnik 1: %s\n", defaultDictionary->defDictionaries[i]);
			i++;
			printf("Recnik 2: %s\n", (defaultDictionary->defDictionaries[i]));
			i++;
			printf("Recnik 3: %s\n", (defaultDictionary->defDictionaries[i]));
			i++;
			printf("Recnik 4: %s\n", (defaultDictionary->defDictionaries[i]));
			i = 0;

			printf("\nUnesite broj recnika: ");
			scanf("%d", &i);
			flush = getchar();
			i--;

			if (i< 0 || i >= defaultDictionary->size) {
				fprintf(stderr, "\nWrong dictionary number\n");
				printf("Unesite 0 za kraj programa: ");
				scanf_s("%d", &end);
				continue;
			}
			currentDictionary = (char*)malloc(sizeof(char)*strlen((defaultDictionary->defDictionaries[i])));
			if (!currentDictionary) {
				free(filename);
				fprintf(stderr, "\ncurrentDictinary memory allocation error\n");
				printf("Unesite 0 za kraj programa: ");
				scanf_s("%d", &end);
				continue;
			}
			currentDictionary = strcpy(currentDictionary, (defaultDictionary->defDictionaries[i]));
			i = 0;
		}
		 
		if (chr == 'N' || chr == 'n') {
			printf("Unesite ime recnika koji zelite da koristite: ");
			filename = readFileName();

			if (!filename) {
				fprintf(stderr, "\nfilename allocation error\n");
				printf("Unesite 0 za kraj programa: ");
				scanf_s("%d", &end);
				continue;
			}

			
			currentDictionary = createUserDictionary(filename, currentDictionary);

			if (!currentDictionary) {
				free(filename);
				fprintf(stderr, "\nUser dictionary error\n");
				printf("Unesite 0 za kraj programa: ");
				scanf_s("%d", &end);
				continue;
			}
		} 

		printf("\n\nUnet je sledeci recnik: %s \n\n\n", currentDictionary);
		printf("Unesite ime trake Tjuringove masine: ");
		free(filename);
		filename = readFileName();
		if (!filename) {
			fprintf(stderr, "\nfilename allocation error\n");
			printf("Unesite 0 za kraj programa: ");
			scanf_s("%d", &end);
			continue;
		}

		printf("\n");
		tapeHeadAndTail = readTape(filename, currentDictionary);
		if (tapeHeadAndTail.error) {
			free(filename);
			fprintf(stderr, "\nTape reading error\n");
			printf("Unesite 0 za kraj programa: ");
			scanf_s("%d", &end);
			continue;

		}

		printf("Ucitana je sledeca traka: ");
		printTape(tapeHeadAndTail);
		printf("\n\n");

		printf("Unesite ime programa za Tjuringovu masinu: ");
		free(filename);
		filename = readFileName();
		
		if (!filename) {
			deleteTapeList(tapeHeadAndTail);
			fprintf(stderr, "\nfilename allocation error\n");
			printf("Unesite 0 za kraj programa: ");
			scanf_s("%d", &end);
			continue;
		}
		printf("\n");
		programHeadAndTail = generateCommandList(filename, currentDictionary);
		if (programHeadAndTail.error) {
			free(filename);
			deleteTapeList(tapeHeadAndTail);
			fprintf(stderr, "\nCommand reading error\n");
			printf("Unesite 0 za kraj programa: ");
			scanf_s("%d", &end);
			continue;
		}
		free(filename);

		if (linkCommandList(programHeadAndTail)) {
			deleteTapeList(tapeHeadAndTail);
			deleteCommandList(programHeadAndTail);
			fprintf(stderr, "\nCommand list linking error\n");
			printf("Unesite 0 za kraj programa: ");
			scanf_s("%d", &end);
			continue;
		}

		initCurrentState(&internalState, programHeadAndTail);
		programTracker = programHeadAndTail.head;

		while (isOver(internalState)) {

			programInput = commandReader(programTracker, internalState, tapeHeadAndTail);

			if (!programInput) {
				deleteTapeList(tapeHeadAndTail);
				deleteCommandList(programHeadAndTail);
				fprintf(stderr, "\nCommand reading error\n");
				printf("Unesite 0 za kraj programa: ");
				scanf_s("%d", &end);
				continue;
			}

			programTracker = programInput->nextState;
			executeCommand(&tapeHeadAndTail, *programInput, &internalState);
		}

		printf("Rezultat programa: ");
		printf("%c\n\n", (internalState.currentState == -1) ? '+' : '-');
		printTape(tapeHeadAndTail);
		printf("\n\n");

		printf("\nProgram je uspesno zavrsen, unesite 0 za kraj: ");
		scanf_s("%d", &end);

		deleteTapeList(tapeHeadAndTail); 
		deleteCommandList(programHeadAndTail);
		//free(currentDictionary);
	} //while(end)
}