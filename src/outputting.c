// standard headers
#include <stdio.h>

// made headers
#include "outputting.h"
#include "memory.h"
#include "registers.h"

// private functions
static void outputGeneralRegisters(void) {
}

static void outputProgramCounter(void) {
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
