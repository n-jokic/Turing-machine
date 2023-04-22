#include "commandReader.h"

NState* commandReader(OState* tempO, State currentState, TapeHAT tapeHat) {
	char chr = tapeReader(tapeHat);
	int state = currentState.currentState;
	OState* tempN = tempO;

	if(tempO->oldState == state) 
	for (int i = 0; i < tempO->numOfNewStates; i++) {
		if ((*(tempO->newStates + i))->oldChar == chr) {
			return *(tempO->newStates + i);
		}
	}
	else {
		while (!tempN && (tempN->oldState != state)) {
			tempN = tempN->next;
		}
		if (!tempN) {
			printf("Greska, nije definisano pocetno stanje");
			return NULL;
		}
		return commandReader(tempN, currentState, tapeHat);
	}

	printf("Ne postoji definisana komanda za q%d i %c\n", state, chr);
	return NULL;
}