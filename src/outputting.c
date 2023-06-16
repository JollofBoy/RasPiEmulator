// standard headers
#include <stdio.h>
#include <stdbool.h>

// made headers
#include "outputting.h"
#include "memory.h"
#include "registers.h"

// private file pointer
static FILE *fptr;

// private functions
static void outputGeneralRegisters(void) {
    fprintf(fptr, "Registers:\n");
    for (int i=0; i<GENERAL_REG_NUM; i++) {
        fprintf(fptr, "X%02d    = %016lx\n", i, readXn(i)); 
    }
}

static void outputProgramCounter(void) {
    fprintf(fptr, "PC     = %016lx\n", getProgramCounter());
}

static void outputPSTATE(void) {
    char n = getN() ? 'N' : '-' ;
    char z = getZ() ? 'Z' : '-' ;
    char c = getC() ? 'C' : '-' ;
    char v = getV() ? 'V' : '-' ;
    fprintf(fptr, "PSTATE : %c%c%c%c\n", n, z, c, v);
}

static void outputNonZeroMemory(void) {
    fprintf(fptr, "Non-zero memory:\n");
    for (int address=0; address<MEM_SIZE; address+=4) {
        if (memoryRead32(address) != 0) {
            fprintf(fptr, "0x%08x: 0x%08x\n", address, memoryRead32(address));
        }
    }
}

bool output(char *outputFile) {
    if (outputFile[0] == 'p') { // if 'p' is parsed, then the pointer points to stdout
        fptr = stdout;
    } else { // otherwise opens the file in write mode
        fptr = fopen(outputFile, "w");
        if (fptr == NULL) {
            return false;
        }
    }

    outputGeneralRegisters();
    outputProgramCounter();
    outputPSTATE();
    outputNonZeroMemory();
    
    // closes the file
    fclose(fptr);

    // just to show that the outputting was successful
    return true;
}
