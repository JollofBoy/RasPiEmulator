// standard headers
#include <stdint.h>
#include <stdbool.h>

// made headers
#include "registers.h"
#include "utils.h"

// general registers
static uint64_t generalRegisters[GENERAL_REG_NUM];

// PSTATE
// had to initialise the pstate here cos it doesn't seem to stick during compile time (may not be the case)
static pstate_t pstate = {.negativeFlag = false, .zeroFlag = true, .carryFlag = false, .overflowFlag = false};

// special registers
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
    return generalRegisters[n] & THIRTYTWO_BIT_MASK;
}

void writeWn(uint32_t value, int registerN) {
    generalRegisters[registerN] = THIRTYTWO_BIT_MASK & value;
}

// register initialise function here 
void registerInitialise(void) {
    for (int i=0; i<GENERAL_REG_NUM; i++) {
        generalRegisters[i] = 0;
    }

    zeroRegister = 0;
    programCounter = 0;
}

// getting the value of the zeroRegister
uint64_t getZeroRegister(void) {
    return zeroRegister;
}

// getting the value of the program counter
uint64_t getProgramCounter(void) {
   return programCounter; 
}

// increments the program counter by n bytes 
void incrementProgramCounter(int n) {
    programCounter += n;
}

// changes the boolean value of the flag
void toggleN(void) {
    pstate.negativeFlag = !(pstate.negativeFlag);
}

void toggleZ(void) {
    pstate.zeroFlag = !(pstate.zeroFlag);
}

void toggleC(void) {
    pstate.carryFlag = !(pstate.carryFlag);
}

void toggleV(void) {
    pstate.overflowFlag = !(pstate.overflowFlag);
}

bool getN(void) {
    return pstate.negativeFlag;
}

bool getZ(void) {
    return pstate.zeroFlag;
}

bool getC(void) {
    return pstate.carryFlag;
}

bool getV(void) {
    return pstate.overflowFlag;
}
