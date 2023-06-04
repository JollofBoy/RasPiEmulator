// standard headers
#include <stdio.h>
#include <stdbool.h>

// made headers
#include "outputting.h"
#include "memory.h"
#include "registers.h"

// private functions
static void outputGeneralRegisters(void) {
    printf("Registers:\n");
    for (int i=0; i<GENERAL_REG_NUM; i++) {
        printf("X%02d    = %016lx\n", i, readXn(i)); 
    }
}

static void outputProgramCounter(void) {
    printf("PC     = %016lx\n", getProgramCounter());
}

static void outputPSTATE(void) {
    char n = getN() ? 'N' : '-' ;
    char z = getZ() ? 'Z' : '-' ;
    char c = getC() ? 'C' : '-' ;
    char v = getV() ? 'V' : '-' ;
    printf("PSTATE : %c%c%c%c\n", n, z, c, v);
}

static void outputNonZeroMemory(void) {
    printf("Non-zero memory:\n");
    for (int address=0; address<MEM_SIZE; address+=4) {
        if (memoryRead32(address) != 0) {
            printf("0x%08x: 0x%08x\n", address, memoryRead32(address));
        }
    }
}

void output(void) {
    outputGeneralRegisters();
    outputProgramCounter();
    outputPSTATE();
    outputNonZeroMemory();
}
