#include "commandExecution.h"

int executeCommand(TapeHAT* tempHat, NState tempState, State* currentState) {
	int move = tempState.move;
	char newChar = tempState.newChar;
	Tape* head = tempHat->head, *tail = tempHat->tail, *readPos = tempHat->readPos;
	TapeHAT temp;
	

	readPos->chr = newChar;
	changeCurrentState(currentState, tempState.newState);
	
	temp = setPosition(*tempHat, move);
	if (temp.error) {
		return 1;
	}

	*tempHat = temp;
	return 0;
}