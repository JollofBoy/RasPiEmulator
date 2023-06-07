// made headers
#include "loadAndStore.h"
#include "dataProcessingImmediate.h"
#include "decode.h"
#include "memory.h"
#include "registers.h"

// private functions

static uint64_t getMemAddressToAccess(uint64_t baseRegister, uint8_t unsignedOffsetFlag,
        uint64_t imm12Val, uint8_t bit21Val, uint64_t xmVal, uint8_t IVal, int64_t simm9Val,
        uint8_t bitWidth /*not needed now but later for defensive programming on vals of imm12*/
       ) {
    // the memory address that is to be accessed
    uint64_t memoryAddressToAccess;
    
    // here we want to use if-else statements to get to execute what we need
    if (unsignedOffsetFlag == 1) { /*Unsigned Immediate Offset*/
        // There are differences in the types of value that imm12 has but this seems to already be
        // handled when passed (defensive coding later)
        memoryAddressToAccess = readXn(baseRegister) + imm12Val; 

    } else if (bit21Val == 1) { /*Register Offset*/
       memoryAddressToAccess = baseRegister + xmVal; 

    } else if (IVal == 1) { /*Pre-Indexed*/
        // memory address is the value of the base register + simm9
        // also replaces the xn register with this sum
        memoryAddressToAccess = readXn(baseRegister) + simm9Val;
        writeXn(memoryAddressToAccess, baseRegister);

    } else { /*Post-Indexed*/
        // memory address as the value of the base register
        memoryAddressToAccess = readXn(baseRegister);
        // then set xn to the address + simm19
        writeXn(memoryAddressToAccess + simm9Val, baseRegister);
    }

    return memoryAddressToAccess;
}

static void load(uint64_t addr, uint64_t targetRegister, uint8_t bitWidth) {
    // as this is always a load, we will store the data in here into the register
    (bitWidth == 32) ? writeWn(memoryRead32(addr), targetRegister) 
        : writeXn(memoryRead64(addr), targetRegister);
}

static void store(uint64_t addr, uint64_t targetRegister, uint8_t bitWidth) {
    (bitWidth == 32) ? memoryWrite32(readWn(targetRegister), addr) 
        : memoryWrite64(readXn(targetRegister), addr);
}

static void loadOrStore(uint64_t addr, uint64_t targetRegister, uint8_t bitWidth, uint8_t lFlag) {
    switch (lFlag) {
        case 1: /*load*/
            load(addr, targetRegister, bitWidth);  
            break;
        case 2: /*store*/
            store(addr, targetRegister, bitWidth);
            break;
    }
}

void executeLAS(void) {
    // sf denotes the size of the load. 0 means target register is 32-bit, 1 means 64-bit
    // L denotes the type of data transfer. 1 means load operation, 0 means store
    // U is an unsigned offset flag. 1 means addressing mode is an Unsigned Offset
    // offset depends on the addressing mode
    // xn is the base register where it has a base address to calculate offsets from 
    // This is always 64-bits
    
    // don't forget that rt is the target register
    
    // first we split this into the Single Data Transfer and Load Literal instructions
   
    // when sf == 0, rt becomes 32-bit
    uint32_t width = (instructionPtr->sf == 0) ? 32 : 64;
    uint64_t memAddressToAccess;

    switch (instructionPtr->bit31) {
        case 1: /*Single Data Transfer*/
            // here the memory address that we are going to access is called here
            memAddressToAccess = getMemAddressToAccess(instructionPtr->xn,
                    instructionPtr->U, instructionPtr->offset->imm12,
                    instructionPtr->offset->bit21, instructionPtr->offset->xm,
                    instructionPtr->offset->I, instructionPtr->offset->simm9, width);

            // here we will do different operations depending on the value of L
            loadOrStore(memAddressToAccess, instructionPtr->rt, width, instructionPtr->L);

            break;
        case 0: /*Load Literal*/
            // this is a simple calculation so no need for another function
            // this part might be a bit shaky
            memAddressToAccess = getProgramCounter() + 
                ((int64_t) instructionPtr->simm19 * 4); 

            // as this is always a load, we will store the data in here into the register
            load(memAddressToAccess, instructionPtr->rt, width);
           
            break;
    }
    


}
