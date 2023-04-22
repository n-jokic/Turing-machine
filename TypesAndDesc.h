#pragma once
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

typedef struct DictSize {
	char** Dictionary;
	int Size;
	int CurSize;
	int Err;
} DSize;

typedef struct DataList {
	char* word;
	struct DataList* Next, *Prev;
	int dictSize;
} DList;

typedef struct HeadAndTail {
	DList* head, *tail;
	int Err;
} HAT;

typedef struct TMProgramState{
	int State;
	char* OldWord, NewWord;
	char Move;
} TMState;

typedef struct TMProgramList {
	int State;
	TMState** Child;
} TMList;

typedef struct TMHeadAndTail {
	TMList* head, *tail;
	int Err;
} TMHAT;



DSize WriteDict(DSize, const char*);
void KillDict(DSize);
DSize TrunckDict(DSize);
HAT AddToList(HAT, const char*);
void KillList(HAT);
char* FormWord(char*, int, char);
char* fromDataWord(FILE*file, char temp, char*word, int i, int flag);
TMHAT TMAddToList(TMHAT, char* , char* , int , int , int );
TMHAT searchList(TMHAT, int);