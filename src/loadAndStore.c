// made headers
#include "loadAndStore.h"
#include "decode.h"
#include "memory.h"
#include "registers.h"

// private functions
static uint64_t getMemAddressToAccess(uint8_t unsignedOffsetFlag, uint64_t baseRegister,
        uint8_t bit21Val, uint64_t imm12Val, uint8_t bitWidth, uint8_t IVal, int32_t simm9Val) {
    // the memory address that is to be accessed
    uint64_t memoryAddressToAccess;
    
    // here we want to use if-else statements to get to execute what we need
    if (unsignedOffsetFlag == 1) { /*Unsigned Immediate Offset*/
        
        // There are differences in the types of value that imm12 has but this seems to already be
        // handled when passed (defensive coding later)
        memoryAddressToAccess = readXn(baseRegister) + imm12Val; 
    } else if (bit21Val == 1) { /*Register Offset*/
        
    } else if (IVal == 1) { /*Pre-Indexed*/
        memoryAddressToAccess = readXn(baseRegister) + simm9Val;
    } else { /*Post-Indexed*/

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
   
    // when sf == 0, rd is accessed as wd
    uint32_t width = (instructionPtr->sf == 0) ? 32 : 64;

    switch (instructionPtr->bit31) {
        case 1: /*Single Data Transfer*/
            // here we are going to do stuff with SDT 
            uint64_t memAddressToAccess = getMemAddressToAccess();
            break;
        case 0: /*Load Literal*/
            // load literal
            break;
    }

}
