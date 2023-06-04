// standard headers
#include <stdio.h>

// made headers
#include "outputting.h"
#include "memory.h"
#include "registers.h"

// private functions
static void outputGeneralRegisters(void) {
    printf("Registers:\n");
    for (int i=0; i<GENERAL_REG_NUM; i++) {
        printf("X%02d = %016lx\n", i, readXn(i)); 
    }
}

static void outputProgramCounter(void) {
    printf("PC  = %016lx\n", getProgramCounter());
}

static void outputPSTATE(void) {
}

static void outputNonZeroMemory(void) {
}

void output(void) {
    outputGeneralRegisters();
    outputProgramCounter();
    outputPSTATE();
    outputNonZeroMemory();
}
