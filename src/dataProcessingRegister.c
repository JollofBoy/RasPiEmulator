// standard headers
#include <stdint.h>

// made headers
#include "dataProcessingRegister.h"
#include "dataProcessingImmediate.h"
#include "decode.h"
#include "memory.h"
#include "registers.h"

// private functions

static uint64_t shiftRm(uint64_t memoryVal, uint8_t shiftType, uint64_t shiftAmount, 
        uint8_t bitWidth) {
    // the result to be returned
    uint64_t result;

    switch (shiftType) {
        case 0x0: /*lsl*/ // logical shift left
            result = (memoryVal << shiftAmount) & activeMask(bitWidth); 
            break;
        case 0x1: /*lsr*/ // logical shift right
            result = (memoryVal & activeMask(bitWidth)) >> shiftAmount;
            break;
        case 0x2: ; /*asr*/ // arithmetic shift right
            // if this is a 1, then the mask is used, otherwise it isn't
            uint8_t mostSigBit = signBitOf(memoryVal, bitWidth);            

            // creates a mask for if the most significant bit is a 1
            uint64_t maskForAsr =
                SIXTYFOUR_BIT_MASK >> (64 - shiftAmount) << (bitWidth - shiftAmount);
            
            result = (memoryVal & activeMask(bitWidth)) >> shiftAmount;
            result = (mostSigBit == 1) ? result | maskForAsr : result;
            break;
        case 0x3: ; /*ror*/ // rotate right // only valid for logical instructions
            // create a mask that takes the bits that are removed as a result of the right shift
            uint64_t maskForRor = SIXTYFOUR_BIT_MASK >> (64 - shiftAmount);
            uint64_t bitsToAddToResult = (memoryVal & activeMask(bitWidth) & maskForRor) 
                << (bitWidth - shiftAmount);

            // the result is found by adding the bits to add via or
            result = (memoryVal & activeMask(bitWidth)) >> shiftAmount | bitsToAddToResult;
            break;
    }
    
    return result;
}

static uint64_t logicOpOn(uint64_t operand1, uint64_t operand2, uint8_t opcode, uint8_t n, 
        uint8_t bitWidth) {
    // the result to be returned for ands and bics
    uint64_t result;

    // checking for the different combinations of opc and N
    if (opcode == 0x0 && n == 0) { /*and*/
        result = operand1 & operand2;
    } else if (opcode == 0x0 && n == 1) { /*bic*/
        result = operand1 & (~operand2);
    } else if (opcode == 0x1 && n == 0) { /*orr*/
        result = operand1 | operand2;
    } else if (opcode == 0x1 && n == 1) { /*orn*/
        result = operand1 | (~operand2);
    } else if (opcode == 0x2 && n == 0) { /*eor*/
        result = operand1 ^ operand2;
    } else if (opcode == 0x2 && n == 1) { /*eon*/
        result = operand1 ^ (~operand2);
    } else if (opcode == 0x3 && n == 0) { /*ands*/
        result = operand1 & operand2;
        // setting the flags

        // if the sign bit is 1, then the number is negative, so set the negative flag to 1
        (signBitOf(result, bitWidth) == 0x1) ? setN(1) : setN(0);

        // when the result is 0, then we set the zero flag to 1
        (result == 0) ? setZ(1) : setZ(0);

        // these two are always set to 0
        setC(0);
        setV(0);
    } else { /*bics*/
        result = operand1 & (~operand2);
        // setting the flags

        // if the sign bit is 1, then the number is negative, so set the negative flag to 1
        (signBitOf(result, bitWidth) == 0x1) ? setN(1) : setN(0);

        // when the result is 0, then we set the zero flag to 1
        (result == 0) ? setZ(1) : setZ(0);

        // these two are always set to 0
        setC(0);
        setV(0);
    }

    return result;
}

static uint64_t multOpOn(uint64_t raVal, uint64_t rnVal, uint64_t rmVal, uint8_t x) {
    if (x == 0) {    
        return raVal + (rnVal * rmVal); 
    } else {
        return raVal - (rnVal * rmVal);
    }
}

// public functions

void executeDPR(void) {
    // if the instruction has a shift, then it is performed on Rm instead of Rd
   
    // sf is the bit-width for all the registers. 0 for 32-bit
    uint8_t width = (instructionPtr->sf == 0) ? 32 : 64;

    // shift the memory register to what it needs to be
    uint64_t rmValue = readFromRegister(instructionPtr->rm, width);
    uint64_t rnValue = readFromRegister(instructionPtr->rnInstruct, width);

    uint64_t shiftedRmVal = shiftRm(rmValue, instructionPtr->opr->shift, 
            instructionPtr->operand->imm6, width);

    if (instructionPtr->M == 0 && instructionPtr->opr->msb == 1) { /*Arithmetic*/
        uint64_t arithResult = arithOpOn(rnValue, shiftedRmVal, instructionPtr->opc, width);

        // writes the result of the arithmetic operation to rd
        writeToRegister(arithResult, instructionPtr->rd, width);
        
    } else if (instructionPtr->M == 0 && instructionPtr->opr->msb == 0) { /*Bit-Logic*/
        uint64_t logicResult = logicOpOn(rnValue, shiftedRmVal, instructionPtr->opc,
                instructionPtr->opr->N, width);

        // writes the result of the logic operation to rd
        writeToRegister(logicResult, instructionPtr->rd, width);

    } else { /*Multiply*/
        // finds the result of applying a multiplication of some sort
        uint64_t raValue = readFromRegister(instructionPtr->operand->ra, width);
        uint64_t multResult = multOpOn(raValue, rnValue, rmValue, instructionPtr->operand->x);

        // writes the result of the multiplication operation to rd
        writeToRegister(multResult, instructionPtr->rd, width);
    }
}
