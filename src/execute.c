// standard headers
#include <stdio.h>

// made headers
#include "decode.h"
#include "execute.h"
#include "memory.h"
#include "registers.h"

void executeInstruction(int instruction /* might make this an enum later, maybe not cos enums are ints as well */) {
    // we are just incrementing for now and this will be updated further later
    incrementProgramCounter(NEXT_INSTRUCTION);
    // the very last thing to do is free the pointer to the struct
    deleteInstructStruct(instructionPtr);
}
