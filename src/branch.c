// standard header
#include <stdio.h>

// made headers
#include "branch.h"
#include "decode.h"
#include "memory.h"
#include "registers.h"

// private functions

// unconditionally updates the Program Counter with an immediate value
static void uUpdatePC(int64_t simmVal) {
    incrementProgramCounter(simmVal * 4);
}

// conditionally updates the Program Counter
static void cUpdatePC(int64_t simmVal, uint8_t condition) {

    // depending on the combination of condition and the flag status, we increment the PC
    if (condition == 0x0 && getZ() == 1) { /*EQ*/
        //TODO
        printf("EQ");
        incrementProgramCounter(simmVal * 4);

    } else if (condition == 0x1 && getZ() == 0) { /*NE*/
        //TODO
        //TODO PHYSICALLY WRITE OUT WHAT IT SAYS SO I CAN SEE AND CROSS_REFERENCE WITH THE OUTPUT
        printf("NE\n");
        incrementProgramCounter(simmVal * 4);
        // TODO this is just to see how NE affects everything somehow
        //TODO
        printf("CONDITIONAL simmVal  for NE is : %ld\n", simmVal);

    } else if (condition == 0xa && getN() == getV()) { /*GE*/
        //TODO
        printf("GE");
        incrementProgramCounter(simmVal * 4);

    } else if (condition == 0xb && getN() != getV()) { /*LT*/
        //TODO
        printf("LT");
        incrementProgramCounter(simmVal * 4);

    } else if (condition == 0xc && getZ() == 0 && getN() == getV()) { /*GT*/
        //TODO
        printf("GT");
        incrementProgramCounter(simmVal * 4);

    } else if (condition == 0xd && !(getZ() == 0 && getN() == getV())) { /*LE*/
        //TODO
        printf("LE");
        incrementProgramCounter(simmVal * 4);

    } else if (condition == 0xe) { /*AL*/
        //TODO
        printf("AL");
        incrementProgramCounter(simmVal * 4);

    } else { /*Just increment the program counter regularly*/
        //TODO
        printf("REGULAR INCREASE");
        incrementProgramCounter(NEXT_INSTRUCTION);
    }
}

// unconditionally updates the Program Counter with a register
static void rUpdatePC(uint64_t reg) {
    int64_t diffBtwnPCAndReg = readXn(reg) - getProgramCounter();
    incrementProgramCounter(diffBtwnPCAndReg);
}

void executeB(void) {
    // depending on the 30 and 31st bit, we would know what type of branch that we are dealing with
    switch (instruction.bits30To31) {
        case 0x0: /*Unconditional*/
            // TODO
            printf("Unconditional");
            uUpdatePC(instruction.simm26);
            break;
        case 0x1: /*Conditional*/
            // TODO
            printf("Conditional");
            cUpdatePC(instruction.simm19, instruction.cond);
            break;
        case 0x3: /*Register*/
            // TODO
            printf("Register");
            rUpdatePC(instruction.xn);
            break;
    }
}
