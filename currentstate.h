#pragma once
#include "command.h"

typedef struct State {
	int currentState;
}State;
void changeCurrentState(State*, int);
int isOver(State);
void initCurrentState(State*, HATS);