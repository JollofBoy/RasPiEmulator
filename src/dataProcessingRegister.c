// standard headers
#include <stdint.h>
// for some reason idk how to link the math header as it is not standard and idk cbuild like that
//#include <math.h>

// made headers
#include "dataProcessingRegister.h"
#include "dataProcessingImmediate.h"
#include "decode.h"
#include "memory.h"
#include "registers.h"

// for the meantime, use the utility function of pow and then once I understand linking better, then we can switch back
#include "utils.h"

// private functions

static uint64_t shiftRm(uint64_t memoryRegister, uint32_t shiftType, uint64_t shiftAmount, uint8_t bitWidth) {
    // the result to be returned
    uint64_t result;

    switch (shiftType) {
        case 0x0: /*lsl*/ // logical shift left
            result = (memoryRegister << shiftAmount) & activeMask(bitWidth); 
            break;
        case 0x1: /*lsr*/ // logical shift right
            result = (memoryRegister & activeMask(bitWidth)) >> shiftAmount;
            break;
        case 0x2: ; /*asr*/ // arithmetic shift right
            // if this is a 1, then the mask is used, otherwise it isn't
            uint8_t mostSigBit = signBitOf(memoryRegister, bitWidth);            

            // creates a mask for if the most significant bit is a 1
            uint64_t maskForAsr = (power(2, shiftAmount) - 1) << (bitWidth - shiftAmount);
            
            result = (memoryRegister & activeMask(bitWidth)) >> shiftAmount;
            result = (mostSigBit == 1) ? result | maskForAsr : result;
            break;
        case 0x3: ; /*ror*/ // rotate right // only valid for logical instructions
            // create a mask that takes the bits that are removed as a result of the right shift
            uint64_t maskForRor = power(2, shiftAmount) - 1;
            uint64_t bitsToAddToResult = (memoryRegister & activeMask(bitWidth) & maskForRor) << (bitWidth - shiftAmount);

            // the result is found by adding the bits to add via or
            result = (memoryRegister & activeMask(bitWidth)) >> shiftAmount | bitsToAddToResult;
            break;
    }
    
    return result;
}

// public functions

void executeDPR(void) {
    // if the instruction has a shift, then it is performed on Rm instead of Rd
    // sf is the bit-width for all the registers. 0 for 32-bit
    uint32_t width = (instructionPtr->sf == 0) ? 32 : 64;

    if (instructionPtr->M == 0 && instructionPtr->opr->msb == 1) { /*Arithmetic*/
        // shift the memory register to what it needs to be
        uint64_t workingRm = shiftRm(instructionPtr->rm, instructionPtr->opr->shift, instructionPtr->operand->imm6, width);
        uint64_t arithResult = arithOpOn(instructionPtr->rnInstruct, workingRm, instructionPtr->opc, width);

        // writes the result of the arithmetic operation to rd
        writeToRegister(arithResult, instructionPtr->rd, width);
        
    } else if (instructionPtr->M == 0 && instructionPtr->opr->msb == 0) { /*Bit-Logic*/
        // shift the memory register to what it needs to be
        //uint64_t workingRm = shiftRm(instructionPtr->rm, instructionPtr->opr->shift, instructionPtr->operand->imm6, width);
        
    } else { /*Multiply*/
        // we will do the changes that gotta happen

    }
}
