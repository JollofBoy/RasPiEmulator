// standard headers
#include <stdio.h>

// made headers
#include "decode.h"
#include "execute.h"
#include "memory.h"
#include "registers.h"

// private functions

void executeInstruction(int instrGroup /* matches on the group of the instruction */) {

    // we are doing a switch case on the different types of instructions
    switch (instrGroup) {
        case DP_IMMEDIATE:
            // call on executeDPI
            incrementProgramCounter(NEXT_INSTRUCTION);
            break;
        case DP_REGISTER:
            // call on executeDPR
            incrementProgramCounter(NEXT_INSTRUCTION);
            break;
        case LOADS_AND_STORES:
            // call on executeLAS
            incrementProgramCounter(NEXT_INSTRUCTION);
            break;
        case BRANCHES:
            // call on executeB
            // here we would increment the program counter accordingly
            break;
        case -1:
            incrementProgramCounter(NEXT_INSTRUCTION);
            break;
        default:
            printf("NO INSTRUCTION DETECTED. recieved:%d", instrGroup);
            incrementProgramCounter(NEXT_INSTRUCTION); /*TODO will remove this later*/
    }

    // the very last thing to do is free the pointer to the struct to ensure minimal memory usage
    deleteInstructStruct(instructionPtr);
}
