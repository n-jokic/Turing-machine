#include "currentstate.h"
#include "command.h"

void initCurrentState(State* currentState, HATS tempHat) {
	currentState->currentState = tempHat.head->oldState; // Prvo stanje 
}

void changeCurrentState(State* currentState, int newState) {
	currentState->currentState = newState;
}

int isOver(State currentState) {
	if (currentState.currentState == -1) return 0;
	if (currentState.currentState == -2) return 0;

	return 1;
}