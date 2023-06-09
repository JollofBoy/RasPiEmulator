// standard headers
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// made headers
#include "dataProcessingImmediate.h"
#include "decode.h"
#include "memory.h"
#include "registers.h"

// private definitions
#define CLEAR32_BITS_MASK 0xffff0000
#define CLEAR64_BITS_MASK 0xffff000000000000
#define MAX_SHIFT64 48
#define MAX_SHIFT32 16

// private functions
//
static uint64_t makeMaskToClearBits(uint8_t bitWidth, uint8_t shiftVal) {
    return (bitWidth == 32) ? 
        CLEAR32_BITS_MASK >> (MAX_SHIFT32 - shiftVal) 
        : CLEAR64_BITS_MASK >> (MAX_SHIFT64 - shiftVal);
}

static void move(uint8_t opcode, uint64_t op, uint64_t dest, uint8_t bitWidth, uint8_t shiftVal) {
    switch (opcode) {
        case 0x0: /*movn*/
            writeToRegister(~op, dest, bitWidth);
            break;
        case 0x2: /*movz*/
            writeToRegister(op, dest, bitWidth);
            break;
        case 0x3: ; /*movk*/
            // I need to understand how this works properly
            uint64_t valueToParse = readXn(dest);
            // has to be the shifted value + 1 so that the 
            uint64_t maskToClearBitsInRange = ~(makeMaskToClearBits(bitWidth, shiftVal));
            
            // this replaces the range that op occupies with op
            valueToParse = (valueToParse & maskToClearBitsInRange) | op;

            // WARNING: parsing the value here means that the top 32 bits will be wiped if sf is 0
            writeToRegister(valueToParse, dest, bitWidth);
            break;
    }
}

// public functions

void executeDPI(void) {
    // sf means bit-width of all registers in instruction. 0 for 32-bit, 1 for 64-bit
    // when sf == 0, rd is accessed as wd
    uint8_t width = (instructionPtr.sf == 0) ? 32 : 64;
    
    // opi determines the type of data processing operation.
    // 010 means Arithmetic instruction, 101 means Wide Move
    switch (instructionPtr.opi) {
        case 0x2: ; /*Arithmetic instruction*/
                  // the semi colon is there to remove the declaration error
            uint8_t shift12 = instructionPtr.operand.sh * 12;
            uint64_t workingImm12 = instructionPtr.operand.imm12 << shift12;
            uint64_t workingOperand = readFromRegister(instructionPtr.operand.rnOperand, width);
            uint64_t arithResult = arithOpOn(workingOperand, workingImm12, 
                    instructionPtr.opc, width);

            writeToRegister(arithResult, instructionPtr.rd, width);
            break;
        case 0x5: ; /*Wide Move*/
            uint8_t shift16 = instructionPtr.operand.hw * 16;
            uint64_t workingImm16 = instructionPtr.operand.imm16 << shift16;
            move(instructionPtr.opc, workingImm16, instructionPtr.rd, width, shift16);
            break;
    }
}

// returns the result of an arith operation
uint64_t arithOpOn(uint64_t operand1, uint64_t operand2, uint8_t opcode, uint8_t bitWidth) {
    // the result to be passed to rd
    uint64_t result;

    // opcode determines the operation to be performed
    switch (opcode) {
        case 0x0: /*add*/
            // the result to be passed to rd and is adjusted to the correct bit width
            result = (operand1 + operand2) & activeMask(bitWidth);
            break;
        case 0x1: /*adds*/
            result = (operand1 + operand2) & activeMask(bitWidth);
            
            // looking for if there is a carry bit
            uint8_t carryBit = (operand1 > activeMask(bitWidth) - operand2) ? 1 : 0; 

            // setting the flags
            
            // if the sign bit is 1, then the number is negative
            (signBitOf(result, bitWidth) == 0x1) ? setN(1) : setN(0);

            // if the result is 0 and the borrow bit is 0, then we set the zero flag to 1 
            (result == 0 && carryBit == 0) ? setZ(1) : setZ(0);

            // if the carry bit is 1 then we set the carry flag to 1
            // could've have written setC(carryBit); but I wanted to make it clearer
            (carryBit == 1) ? setC(1) : setC(0);

            // if the operands have the same sign but the result has a different sign,
            // then we set the overflow flag to 1 
            bool sameSign = signBitOf(operand1, bitWidth) == signBitOf(operand2, bitWidth);
            (sameSign && signBitOf(operand2, bitWidth) != signBitOf(result, bitWidth)) 
                ? setV(1) : setV(0);
            break;
        case 0x2: /*sub*/
            result = (operand1 - operand2) & activeMask(bitWidth);
            break;
        case 0x3: /*subs*/
            //TODO
            printf("subs");
            result = (operand1 - operand2) & activeMask(bitWidth);

            uint8_t borrowBit = (operand1 < MIN + operand2) ? 1 : 0;

            // setting the flags

            // if the sign bit is 1, then the number is negative, so set the negative flag to 1
            (signBitOf(result, bitWidth) == 0x1) ? setN(1) : setN(0);

            // if the result is 0 and the borrow bit is 0, then we set the zero flag to 1 
            (result == 0 && borrowBit == 0) ? setZ(1) : setZ(0);

            // if the borrow bit is 1, then we set the carry flag to 0
            (borrowBit == 1) ? setC(0) : setC(1);

            bool diffSign = signBitOf(operand1, bitWidth) != signBitOf(operand2, bitWidth);
            (diffSign && signBitOf(operand2, bitWidth) == signBitOf(result, bitWidth)) 
                ? setV(1) : setV(0);
            break;
    }

    return result;
}
