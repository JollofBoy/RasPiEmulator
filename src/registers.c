// standard headers
#include <stdint.h>
#include <stdbool.h>

// made headers
#include "registers.h"
#include "utils.h"

// general registers
static uint64_t generalRegisters[GENERAL_REG_NUM];

// PSTATE
// had to initialise the pstate here cos it doesn't seem to stick during compile time
// (may not be the case)
static pstate_t pstate = {.negativeFlag = false, .zeroFlag = true, 
    .carryFlag = false, .overflowFlag = false};

// is true if any pstate flag has changed
static bool changed = false;

// special registers
static uint64_t zeroRegister;
static int64_t programCounter;

// detects if a flag change has occurs
static void flagChanged(bool ogFlag, bool newVal) {
    if (ogFlag != newVal) {
        changed = true;
    }
}

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

// these functions read and write from or to a register depending on the bitWidth

// writes the result to the destination register
void writeToRegister(uint64_t result, uint64_t destinationRegister, uint8_t bitWidth) {
    (bitWidth == 32) ? 
        writeWn(result & THIRTYTWO_BIT_MASK, destinationRegister) 
        : writeXn(result, destinationRegister);
}

// reads from the register in the correct mode
uint64_t readFromRegister(uint8_t reg, uint8_t bitWidth) {
    return (bitWidth == 32) ? readWn(reg) : readXn(reg);
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
void incrementProgramCounter(int64_t n) {
    programCounter += n;
}

// sets the boolean value of the flag and tells returns true or false
void setN(bool value) {
    flagChanged(pstate.negativeFlag, value);
    pstate.negativeFlag = value;
}

void setZ(bool value) {
    flagChanged(pstate.zeroFlag, value);
    pstate.zeroFlag = value;
}

void setC(bool value) {
    flagChanged(pstate.carryFlag, value);
    pstate.carryFlag = value;
}

void setV(bool value) {
    flagChanged(pstate.overflowFlag, value);
    pstate.overflowFlag = value;
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

void resetChanged(void) {
    changed = false;
}

bool getChanged(void) {
    return changed;
}
