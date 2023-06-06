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

static uint64_t logicOpOn(uint64_t operand1, uint64_t operand2, uint8_t opcode, uint8_t n, uint8_t bitWidth) {
    // the result to be returned for ands and bics
    uint64_t result;

    // checking for the different combinations of opc and N
    if (opcode == 0x0 && n == 0) { /*and*/
        return operand1 & operand2;
    } else if (opcode == 0x0 && n == 1) { /*bic*/
        return operand1 & (~operand2);
    } else if (opcode == 0x1 && n == 0) { /*orr*/
        return operand1 | operand2;
    } else if (opcode == 0x1 && n == 1) { /*orn*/
        return operand1 | (~operand2);
    } else if (opcode == 0x2 && n == 0) { /*eon*/
        return operand1 ^ operand2;
    } else if (opcode == 0x2 && n == 1) { /*eor*/
        return operand1 ^ (~operand2);
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
        uint64_t workingRm = shiftRm(instructionPtr->rm, instructionPtr->opr->shift, instructionPtr->operand->imm6, width);
        uint64_t logicResult = logicOpOn(instructionPtr->rnInstruct, workingRm, instructionPtr->opc, instructionPtr->opr->N, width);

        // writes the result of the logic operation to rd
        writeToRegister(logicResult, instructionPtr->rd, width);

    } else { /*Multiply*/
        // we will do the changes that gotta happen

    }
}
