#pragma once
#include "tape.h"
#include "command.h"
#include "currentstate.h"

int executeCommand(TapeHAT*, NState, State*);