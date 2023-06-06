// standard headers
#include <stdio.h>

// made headers
#include "execute.h"
#include "memory.h"
#include "registers.h"
#include "decode.h"
#include "dataProcessingImmediate.h"
#include "dataProcessingRegister.h"
#include "loadAndStore.h"
#include "branch.h"

void executeInstruction(int instrGroup /* matches on the group of the instruction */) {

    // prints out the contents of the instruction structure
    printInstructStructContents();

    // we are doing a switch case on the different types of instructions
    switch (instrGroup) {
        case DP_IMMEDIATE:
            // call on executeDPI (found in the corresponding file)
            executeDPI();
            incrementProgramCounter(NEXT_INSTRUCTION);
            break;
        case DP_REGISTER:
            // call on executeDPR (found in the corresponding file)
            executeDPR();
            incrementProgramCounter(NEXT_INSTRUCTION);
            break;
        case LOADS_AND_STORES:
            // call on executeLAS (found in the corresponding file)
            executeLAS();
            incrementProgramCounter(NEXT_INSTRUCTION);
            break;
        case BRANCHES:
            // call on executeB (found in the corresponding file)
            // the program counter will be correctly incremented here
            executeB();
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
