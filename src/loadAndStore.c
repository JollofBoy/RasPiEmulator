// standard headers
#include <stdio.h>
// made headers
#include "loadAndStore.h"
#include "decode.h"
#include "memory.h"
#include "registers.h"

// private functions

static uint64_t getMemAddressToAccess(uint64_t baseRegister, uint8_t unsignedOffsetFlag, 
        uint8_t bitWidth, uint64_t imm12Val, uint8_t bit21Val, uint64_t xmVal, uint8_t IVal,
        int64_t simm9Val) {
    // the memory address that is to be accessed
    uint64_t memoryAddressToAccess;
    
    if (unsignedOffsetFlag == 1) { /*Unsigned Immediate Offset*/
        // There are differences in the types of value that imm12 has but this seems to already be
        // handled when passed (defensive coding later)
        uint8_t multiplier = (bitWidth == 32) ? 4 : 8;
        memoryAddressToAccess = readXn(baseRegister) + imm12Val * multiplier;

    } else if (bit21Val == 1) { /*Register Offset*/
        memoryAddressToAccess = readXn(baseRegister) + xmVal; 

    } else if (IVal == 1) { /*Pre-Indexed*/
        // memory address is the value of the base register + simm9
        // this replaces the Xn value
        memoryAddressToAccess = readXn(baseRegister) + simm9Val;
        writeXn(memoryAddressToAccess, baseRegister);

    } else { /*Post-Indexed*/
        // memory address as the value of the base register
        memoryAddressToAccess = readXn(baseRegister);
        writeXn(memoryAddressToAccess + simm9Val, baseRegister);
    }

    return memoryAddressToAccess;
}

static void load(uint64_t addr, uint64_t targetRegister, uint8_t bitWidth) {
    // the bitWidth determines the type of write to do to the target register
    (bitWidth == 32) ? writeWn(memoryRead32(addr), targetRegister) 
        : writeXn(memoryRead64(addr), targetRegister);
}

static void store(uint64_t addr, uint64_t targetRegister, uint8_t bitWidth) {
    // the bitWidth determines the type of write to do to the target regsiter
    (bitWidth == 32) ? memoryWrite32(readWn(targetRegister), addr) 
        : memoryWrite64(readXn(targetRegister), addr);
}

static void loadOrStore(uint64_t addr, uint64_t targetRegister, uint8_t bitWidth, uint8_t lFlag) {
    switch (lFlag) {
        case 1: /*load*/
            load(addr, targetRegister, bitWidth);  
            break;
        case 0: /*store*/
            store(addr, targetRegister, bitWidth);
            break;
    }
}

void executeLAS(void) {
    
    // first we split this into the Single Data Transfer and Load Literal instructions
   
    // when sf == 0, rt becomes 32-bit
    uint32_t width = (instruction.sf == 0) ? 32 : 64;
    uint64_t memAddressToAccess;

    switch (instruction.bit31) {
        case 1: ; /*Single Data Transfer*/
            uint64_t xmVal = readXn(instruction.offset.xm);

            // here the memory address that we are going to access is called here
            memAddressToAccess = getMemAddressToAccess(instruction.xn, instruction.U,
                    width, instruction.offset.imm12, instruction.offset.bit21, xmVal,
                    instruction.offset.I, instruction.offset.simm9);
            
            // here we will do different operations depending on the value of L
            loadOrStore(memAddressToAccess, instruction.rt, width, instruction.L);

            break;
        case 0: /*Load Literal*/
            // this is a simple calculation so no need for another function
            // this part might be a bit shaky
            memAddressToAccess = getProgramCounter() + (instruction.simm19 * 4); 

            // as this is always a load, we will store the data in here into the register
            load(memAddressToAccess, instruction.rt, width);
           
            break;
    }
}
