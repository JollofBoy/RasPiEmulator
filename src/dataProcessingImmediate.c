// standard headers
#include <stdint.h>
#include <stdbool.h>

// made headers
#include "dataProcessingImmediate.h"
#include "decode.h"
#include "memory.h"
#include "registers.h"

// private functions

// returns the mask that we should use for a given function
static uint64_t activeMask(uint8_t bitWidth) {
    return (bitWidth == 32) ? THIRTYTWO_BIT_MASK : SIXTYFOUR_BIT_MASK;
}

// returns the maximum value that we should use for a given function
static uint64_t activeMax(uint8_t bitWidth) {
    return (bitWidth == 32) ? MAX32 : MAX64;
}

// returns the sign bit of a given number
static uint8_t signBitOf(uint64_t value, uint8_t bitWidth) {
    return value >> (bitWidth-1);
}

// returns the shifed or un-shifted version of imm12
static uint64_t getImm12(uint8_t shift, uint64_t immVal, uint8_t bitWidth) {
    if (shift == 1) {
        return (immVal << 12) & activeMask(bitWidth);
    }
    return immVal & activeMask(bitWidth);
}

// takes out the commonality for add instructions
static uint64_t add(uint64_t operand1, uint64_t operand2, uint8_t bitWidth) {
    // the result to be passed to rd and is adjusted to the correct bit width
    uint64_t result = (operand1 + operand2) & activeMask(bitWidth);

    // looking for if there is a carry bit
    uint8_t carryBit = (operand1 > activeMax(bitWidth) - operand2) ? 1 : 0; 
            
    // setting the flags

    // when there is no carry bit and the result is 0, then we set the zero flag to 1
    (result == 0 && carryBit == 0) ? setZ(1) : setZ(0); 
            
    // if the carry bit is 1 then we set the carry flag to 1
    // could've have written setC(carryBit); but I wanted to make it clearer
    (carryBit == 1) ? setC(1) : setC(0);

    return result;
}

// returns the result of an arith operation
static uint64_t arithOpOn(uint64_t operand1, uint64_t operand2, uint8_t opcode, uint8_t bitWidth) {
    // the result to be passed to rd
    uint64_t result;

    // opcode determines the operation to be performed
    switch (opcode) {
        case 0x0: // add
            result = add(operand1, operand2, bitWidth);
            break;
        case 0x1: //adds
            result = add(operand1, operand2, bitWidth);

            // setting the remaining flags
            
            // if the sign bit is 1, then the number is negative
            (signBitOf(result, bitWidth) == 0x1) ? setN(1) : setN(0);

            // if the operands have the same sign but the result has a different sign, then we set the overflow flag to 1 
            bool sameSign = signBitOf(operand1, bitWidth) == signBitOf(operand2, bitWidth);
            (sameSign && signBitOf(operand2, bitWidth) != signBitOf(result, bitWidth)) ? setV(1) : setV(0);
            break;
        case 0x2: //sub
            result = (operand1 - operand2) & activeMask(bitWidth);
            break;
        case 0x3: //subs
            result = (operand1 - operand2) & activeMask(bitWidth);

            uint8_t borrowBit = (operand1 < MIN + operand2) ? 1 : 0;

            // setting the flags

            // if the sign bit is 1, then the number is negative
            (signBitOf(result, bitWidth) == 0x1) ? setN(1) : setN(0);

            // if the result is 0 and the borrow bit is 0, then we set the zero flag to 1 
            (result == 0 && borrowBit == 0) ? setZ(1) : setZ(0);

            // TODO here it says something about if there is a borrow then we set it to 0??? (we will diagnose after testing)
            // if the borrow bit is 1, then we set the carry flag to 1 (for now)
            (borrowBit == 1) ? setC(1) : setC(0);

            bool diffSign = signBitOf(operand1, bitWidth) != signBitOf(operand2, bitWidth);
            (diffSign && signBitOf(operand2, bitWidth) == signBitOf(result, bitWidth)) ? setV(1) : setV(0);
            break;
    }

    return result;
}

void executeDPI(void) {
    // based on rd, we encode zr(so do nothing) if it is 0x1f so it seems we don't need to check for it
    // if the instruction is arithmetic and doesn't set condition flags, sp is encoded(no need to do this)

    // sf means bit-width of all registers in instruction. 0 for 32-bit, 1 for 64-bit
    // when sf == 0, rd is accessed as wd
    uint32_t width = (instructionPtr->sf == 0) ? 32 : 64;
    
    // opi determines the type of data processing operation.
    // 010 means Arithmetic instruction, 101 means Wide Move
    switch (instructionPtr->opi) {
        case 0x2: ; // Arithmetic instruction   // the semi colon is there to remove the declaration error
            uint64_t workingImm12 = getImm12(instructionPtr->operand->sh, instructionPtr->operand->imm12, width);
            uint64_t arithResult = arithOpOn(instructionPtr->operand->rnOperand, workingImm12, instructionPtr->opc, width);
            // writes to the correct width register
            (instructionPtr->sf == 0) ? writeWn(arithResult, instructionPtr->rd) : writeXn(arithResult, instructionPtr->rd);
            break;
        case 0x5: // Wide Move
            break;
    }
}
