#include <stdio.h>
#include "execute.h"
#include "memory.h"
#include "registers.h"

void executeInstruction(int instruction /* might make this an enum later, maybe not cos enums are ints as well */) {
    // we are just incrementing for now and this will be updated further later
    incrementProgramCounter(NEXT_INSTRUCTION);
}
