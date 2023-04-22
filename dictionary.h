#pragma once
#ifndef _STRUCTURES_
#define _STRUCTURES_
#include <stdlib.h>
#define NUM_OF_FUNCTIONS 5
#include <stdio.h>
#define BLANK 'b'
#define CHUNK 10
#define DEFAULT_SIZE 4 
#define DICTIONARY_1 3
#define DICTIONARY_2 4
#define DICTIONARY_3 5
#define DICTIONARY_4 6
#define MAX_FILENAME_SIZE 255

typedef struct Dictionary {
	char** dictList;
	int error;
} Dict;

typedef struct DefaultDictionary {
	int size;
	char defDictionaries[DEFAULT_SIZE][DICTIONARY_4];
}DefDict;

//Funkcije
Dict initDict(Dict, DefDict);
char* createUserDictionary(char*, char**);
Dict readUserDictionary(FILE*, char** ); 
int searchDictionary(char, char*);
void* initDefList(DefDict*); 
DefDict* initDefaultDict();

#endif