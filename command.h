#pragma once
#ifndef _TAPE_
#define _TAPE_
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define LEFT_PARENTH '('
#define RIGHT_PARENTH ')'
#define SEPARATOR ','
#define CONNECTOR '='

typedef struct OldState {
	int oldState;
	int numOfNewStates;
	struct NewState** newStates;
	struct OldState *prev, *next;
	int error;
} OState;

typedef struct NewState {
	int newState;
	char oldChar, newChar;
	int move;
	struct OldState* oldState;
	struct OldState* nextState;
	int error;
}NState;

typedef struct HeadAndTailOldState {
	struct OldState *head, *tail;
	int error;
} HATS;

//Funkcije
HATS generateCommandList(char*, char*);
int linkCommandList(HATS);
int linkNStates(OState*, OState*);
int searchList(NState*, OState*);
HATS formList(HATS *, OState*);
int findPlacement(OState**, OState**, OState**);
int addToMid(OState *, OState**);
int replaceMid(OState*, OState**);
void insertMid(OState*, OState*);
inline void addToBeg(OState**, OState*);
inline void addToEnd(OState**, OState*);
HATS initTempHats(OState); 
void deleteCommandList(HATS);
OState* initTempState();
void freeOState(OState*);
OState readCommand(FILE*, char*, char*, int*);
int linkNStates(OState*, OState*);
OState* initTempOldState(int, int, char, char, int);
int linkNStates(OState*, OState*);
NState* initTempNState();
int isnum(char);
#endif