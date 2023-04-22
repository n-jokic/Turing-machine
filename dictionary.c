#include "dictionary.h"


DefDict* initDefaultDict() {
	char defDictionaries [DEFAULT_SIZE][DICTIONARY_4] = 
	{{ '0', BLANK, '\0' },
	{ '0', '1', BLANK, '\0' },
	{ '0', '1', '2', BLANK ,  '\0' },
	{ '0', '1', '2', '3', BLANK , '\0' }
	};
	int t = 0;

	DefDict* tempDict = malloc(sizeof(DefDict));
	if (!tempDict) {
		return tempDict;
	}

	tempDict->size = DEFAULT_SIZE;


	for (int i = 0; i < DICTIONARY_1; i++) {
		tempDict->defDictionaries[t][i] = defDictionaries[t][i];
	}
	t++;
	for (int i = 0; i < DICTIONARY_2; i++) {
		tempDict->defDictionaries[t][i] = defDictionaries[t][i];
	}
	t++;
	for (int i = 0; i < DICTIONARY_3; i++) {
		tempDict->defDictionaries[t][i] = defDictionaries[t][i];
	}
	t++;
	for (int i = 0; i < DICTIONARY_4; i++) {
		tempDict->defDictionaries[t][i] = defDictionaries[t][i];
	}
	t = 0;

	
	return tempDict;
}

Dict* initDictList() {
	Dict* tempDict = (Dict*)malloc(sizeof(Dict));
	if (!tempDict) {
		return NULL;
	}

	tempDict = NULL;
	return tempDict;
}

char* createUserDictionary(char* fileName, char** temp) {
	char* chPtr = NULL;
	chPtr = (char*)malloc(sizeof(char)*CHUNK);
	FILE* file;
	char c[2] = "r";
	const char* chr = &c;
	Dict tempDict;
	if (!chPtr) {
		return NULL;
	}

	file = fopen(fileName, chr);
	if (!file) {

		free(chPtr);
		return NULL;
	}

	tempDict = readUserDictionary(file, &chPtr);
	if (tempDict.error) return NULL;

	return *(tempDict.dictList);
}


Dict readUserDictionary(FILE* file, char** chPtr){
	int i = 0;
	char tempChr = BLANK;
	char* tempChPtr = NULL;

	while ((tempChr!=EOF )&& ((tempChr=fgetc(file)) != EOF)) {
		if (tempChr == BLANK) continue;
		if (i % CHUNK == 0 && i != 0) {
			tempChPtr = (char*)realloc(*chPtr, sizeof(char)*(CHUNK + i));
			if (!tempChPtr) {
				free(*chPtr);
				fclose(file);
				 return (Dict) { NULL, 1 };
			}
			*chPtr = tempChPtr;
		}
			*(*chPtr + i) = tempChr;
			i++;
	}
	
	if (i % CHUNK == 0 && i != 0) {
		tempChPtr = (char*)realloc(*chPtr, sizeof(char)*(CHUNK + i));
		if (!tempChPtr) {
			free(*chPtr);
			fclose(file);
			return (Dict) { NULL, 1 };
		}
	}
	*(*chPtr + i) = BLANK;
	i++;
	if (i % CHUNK == 0 && i != 0) {
		tempChPtr = (char*)realloc(*chPtr, sizeof(char)*(CHUNK + i));
		if (!tempChPtr) {
			free(*chPtr);
			fclose(file);
			return (Dict) { NULL, 1 };
		}
	}
	*(*chPtr + i) = '\0';

	if (i > MAX_FILENAME_SIZE) {
		free(*chPtr);
		fclose(file);
		return (Dict) { NULL, 1 };
	}
	fclose(file);

	return (Dict) { chPtr, 0 };
}

int searchDictionary(char chr, char* currentDictionary) {
	int i = 0;
	if (chr == BLANK) return 1;
	while (*(currentDictionary + i) != BLANK) {
		if (*(currentDictionary + i) == chr) {
			return 1;
		}
		i++;
	}
	return 0;
}