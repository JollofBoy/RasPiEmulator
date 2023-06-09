// standard headers
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// made headers
#include "decode.h"

// macro for outputting the struct
#define getName(var)  #var

// private definitions
#define OP0_SHIFT 25
#define OP0_MASK 0x0000000f 

// under here include a global variable pointer that will point to the instruction_t struct
instruction_t instructionPtr; // TODO if the allocation doesn't work, just look to initialise evertying

// definining private functions

// these extend the corresponding simm numbers to the correct number
static uint64_t signExtend9(uint64_t num) {
    if (signBitOf(num, 9) == 1) { /*the sign bit on*/
        return NINE_SIGN_EXTENSION_MASK | num; 
    }
    return num;
}

static uint64_t signExtend19(uint64_t num) {
    if (signBitOf(num, 19) == 1) { /*the sign bit on*/
        return NINETEEN_SIGN_EXTENSION_MASK | num; 
    }
    return num;
}

static uint64_t signExtend26(uint64_t num) {
    if (signBitOf(num, 26) == 1) { /*if the number is greater than the upper bound, you negate it*/
        return TWENTYSIX_SIGN_EXTENSION_MASK | num; 
    }
    return num;
}

// these are static as no other file would need to load anything into memory directly
static void loadDPI(uint32_t instr) {
    
    // here we are going to literally assign a value to every piece of info relevant
    instructionPtr.sf = instr >> 31;
    instructionPtr.opc = TWO_BIT_MASK & (instr >> 29);
    instructionPtr.opi = THREE_BIT_MASK & (instr >> 23);
    instructionPtr.rd = FIVE_BIT_MASK & instr;

    instructionPtr.operand.sh = ONE_BIT_MASK & (instr >> 22);
    instructionPtr.operand.imm12 = TWELVE_BIT_MASK & (instr >> 10);
    instructionPtr.operand.rnOperand = FIVE_BIT_MASK & (instr >> 5);
    instructionPtr.operand.hw = TWO_BIT_MASK & (instr >> 21);
    instructionPtr.operand.imm16 = SIXTEEN_BIT_MASK & (instr >> 5);
}

static void loadDPR(uint32_t instr) {

    instructionPtr.sf = instr >> 31;
    instructionPtr.opc = TWO_BIT_MASK & (instr >> 29);
    instructionPtr.M = ONE_BIT_MASK & (instr >> 28);
    instructionPtr.rm = FIVE_BIT_MASK & (instr >> 16);
    instructionPtr.rnInstruct = FIVE_BIT_MASK & (instr >> 5);
    instructionPtr.rd = FIVE_BIT_MASK & instr;

    instructionPtr.operand.imm6 = SIX_BIT_MASK & (instr >> 10);
    instructionPtr.operand.x = ONE_BIT_MASK & (instr >> 15);
    instructionPtr.operand.ra = FIVE_BIT_MASK & (instr >> 10);
    
    instructionPtr.opr.msb = ONE_BIT_MASK & (instr >> 24);
    instructionPtr.opr.shift = TWO_BIT_MASK & (instr >> 22);
    instructionPtr.opr.N = ONE_BIT_MASK & (instr >> 21);
    instructionPtr.opr.val = FOUR_BIT_MASK & (instr >> 21);
}

static void loadLAS(uint32_t instr) {

    instructionPtr.bit31 = instr >> 31;
    instructionPtr.sf = ONE_BIT_MASK & (instr >> 30);
    instructionPtr.U = ONE_BIT_MASK & (instr >> 24);
    instructionPtr.L = ONE_BIT_MASK & (instr >> 22);
    instructionPtr.xn = FIVE_BIT_MASK & (instr >> 5);
    instructionPtr.rt = FIVE_BIT_MASK & instr;
    instructionPtr.simm19 = signExtend19(NINETEEN_BIT_MASK & (instr >> 5));

    instructionPtr.offset.bit21 = ONE_BIT_MASK & (instr >> 21);
    instructionPtr.offset.xm = FIVE_BIT_MASK & (instr >> 16);
    instructionPtr.offset.simm9 = signExtend9(NINE_BIT_MASK & (instr >> 12));
    instructionPtr.offset.I = ONE_BIT_MASK & (instr >> 11);
    instructionPtr.offset.imm12 = TWELVE_BIT_MASK & (instr >> 10);
}

static void loadB(uint32_t instr) {

    instructionPtr.bits30To31 = TWO_BIT_MASK & (instr >> 30);
    instructionPtr.simm26 = signExtend26(TWENTYSIX_BIT_MASK & instr);
    instructionPtr.xn = FIVE_BIT_MASK & (instr >> 5);
    instructionPtr.simm19 = signExtend19(NINETEEN_BIT_MASK & (instr >> 5));
    instructionPtr.cond = FOUR_BIT_MASK & instr;
}

// this is the identifier value to see what group the instruction goes under
static uint8_t op0; 

group_t decodeInstruction(uint32_t instruction) {
    group_t group;
    // we set the op0 to its value
    op0 = OP0_MASK & (instruction >> OP0_SHIFT);

    // finds what the group of the instruction is based on the op0 and then 
    // loads values to the relevant markers
    switch (op0) {
        case 0x8:
        case 0x9:
            group = DP_IMMEDIATE;
            printf("DP IMMEDIATE\n");
            loadDPI(instruction); // this sets the structs to every possible value 
                                       // in the case of DP_IMMEDIATE
            break;
        case 0x5:
        case 0xd:
            group = DP_REGISTER;
            printf("DP REGISTER\n");
            loadDPR(instruction); // this sets the structs to every possible value
                                       // in the case of DP_REGISTER
            break;
        case 0x4:
        case 0x6:
        case 0xc:
        case 0xe:
            group = LOADS_AND_STORES;
            printf("LOADS AND STORES\n");
            loadLAS(instruction); // this sets the structs to every possible value
                                       // in the case of LOADS_AND_STORES
            break;
        case 0xa:
        case 0xb:
            group = BRANCHES;
            printf("BRANCHES\n");
            loadB(instruction); // this sets the structs to every possible value in 
                                     // the case of BRANCHES
            break;
        default:
            printf("NO MATCH. op0 = %d\n", op0);
    }

    return group;
}

void printInstructStructContents(void) {
    printf("%s = %d\n", getName(sf), instructionPtr.sf);
    printf("%s = %d\n", getName(opc), instructionPtr.opc);
    printf("%s = %d\n", getName(opi), instructionPtr.opi);
    printf("%s = %d\n", getName(rd), instructionPtr.rd);
    printf("%s = %d\n", getName(M), instructionPtr.M);
    printf("%s = %d\n", getName(rm), instructionPtr.rm);
    printf("%s = %d\n", getName(rnInstruct), instructionPtr.rnInstruct);
    printf("%s = %d\n", getName(bit31), instructionPtr.bit31);
    printf("%s = %d\n", getName(U), instructionPtr.U);
    printf("%s = %d\n", getName(L), instructionPtr.L);
    printf("%s = %d\n", getName(xn), instructionPtr.xn);
    printf("%s = %d\n", getName(rt), instructionPtr.rt);
    printf("%s = %d\n", getName(cond), instructionPtr.cond);
    printf("%s = %d\n", getName(bits30To31), instructionPtr.bits30To31);
    printf("%s = %ld\n", getName(simm19), instructionPtr.simm19);
    printf("%s = %ld\n", getName(simm26), instructionPtr.simm26);

    printf("Operand.%s = %d\n", getName(sh), instructionPtr.operand.sh);
    printf("Operand.%s = %ld\n", getName(imm12), instructionPtr.operand.imm12);
    printf("Operand.%s = %d\n", getName(rnOperand), instructionPtr.operand.rnOperand);
    printf("Operand.%s = %d\n", getName(hw), instructionPtr.operand.hw);
    printf("Operand.%s = %ld\n", getName(imm16), instructionPtr.operand.imm16);
    printf("Operand.%s = %ld\n", getName(imm6), instructionPtr.operand.imm6);
    printf("Operand.%s = %d\n", getName(x), instructionPtr.operand.x);
    printf("Operand.%s = %d\n", getName(ra), instructionPtr.operand.ra);

    printf("Offset.%s = %d\n", getName(bit21), instructionPtr.offset.bit21);
    printf("Offset.%s = %d\n", getName(xm), instructionPtr.offset.xm);
    printf("Offset.%s = %ld\n", getName(simm9), instructionPtr.offset.simm9);
    printf("Offset.%s = %d\n", getName(I), instructionPtr.offset.I);
    printf("Offset.%s = %ld\n", getName(imm12), instructionPtr.offset.imm12);

    printf("opr.%s = %d\n", getName(msb), instructionPtr.opr.msb);
    printf("opr.%s = %d\n", getName(shift), instructionPtr.opr.shift);
    printf("opr.%s = %d\n", getName(N), instructionPtr.opr.N);
    printf("opr.%s = %d\n", getName(val), instructionPtr.opr.val);
}

// returns a mask ofo the correct bitWidth
uint64_t activeMask(uint8_t bitWidth) {
    return (bitWidth == 32) ? THIRTYTWO_BIT_MASK : SIXTYFOUR_BIT_MASK;
}

// returns the sign bit of a given number
uint8_t signBitOf(uint64_t value, uint8_t bitWidth) {
    return value >> (bitWidth-1);
}
