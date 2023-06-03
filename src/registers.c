// standard headers
#include <stdint.h>
#include <stdbool.h>

// made headers
#include "registers.h"
#include "utils.h"

// creating the variables for the structures
static uint64_t generalRegisters[GENERAL_REG_NUM];
static pstate_t pstate;
static uint64_t zeroRegister;
static uint64_t programCounter;

// 64-bit mode manipulations
uint64_t readXn(int n) {
    return generalRegisters[n];
}

void writeXn(uint64_t value, int registerN) {
    generalRegisters[registerN] = value;
}

// 32-bit mode manipulations
uint32_t readWn(int n) {
    return generalRegisters[n] & 0x00000000ffffffff;
}

void writeWn(uint32_t value, int registerN) {
    generalRegisters[registerN] = 0x00000000ffffffff & value;
}

// register initialise function here 
void registerInitialise(void) {
    for (int i=0; i<GENERAL_REG_NUM; i++) {
        generalRegisters[i] = 0;
    }

    zeroRegister = 0;
    programCounter = 0;
    pstate.negativeFlag = 0;
    pstate.zeroFlag = 0;
    pstate.carryFlag = 0;
    pstate.overflowFlag = 0;
}

// getting the value of the zeroRegister
uint64_t getZeroRegister(void) {
    return zeroRegister;
}

// getting the value of the program counter
uint64_t getProgramCounter(void) {
   return programCounter; 
}

// increments the program counter by n
void incrementProgramCounter(uint64_t n) {
    programCounter += n;
}

// here we would edit the PSTATE in some way if needed
// cos they said there would be cheeky lil edits here
