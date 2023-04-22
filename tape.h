#pragma once
#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct TapeList {
	char chr;
	struct TapeList* next, *prev;
} Tape;

typedef struct TapeListHeadAndTail {
	Tape* head, *tail, *readPos;
	int error;
} TapeHAT;
//Funkcije
Tape* initTempTape();
TapeHAT addToTape(TapeHAT, Tape* );
TapeHAT setPosition(TapeHAT, int);
TapeHAT readTape(char*);
void deleteTapeList(TapeHAT);
TapeHAT setPosition(TapeHAT, int);
char tapeReader(TapeHAT);
void printTape(TapeHAT);

int searchDictionary(char, char*);