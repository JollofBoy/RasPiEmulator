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
instruction_t instruction; 

// definining private functions

// these extend the corresponding simm numbers to the correct number
static int64_t signExtend9(int64_t num) {
    if (signBitOf(num, 9) == 1) { /*the sign bit on*/
        return NINE_SIGN_EXTENSION_MASK | num; 
    }
    return num;
}

static int64_t signExtend19(int64_t num) {
    if (signBitOf(num, 19) == 1) { /*the sign bit on*/
        return NINETEEN_SIGN_EXTENSION_MASK | num; 
    }
    return num;
}

static int64_t signExtend26(int64_t num) {
    if (signBitOf(num, 26) == 1) { /*if the number is greater than the upper bound, you negate it*/
        return TWENTYSIX_SIGN_EXTENSION_MASK | num; 
    }
    return num;
}

// these are static as no other file would need to load anything into memory directly
static void loadDPI(uint32_t instr) {
    
    // here we are going to literally assign a value to every piece of info relevant
    instruction.sf = instr >> 31;
    instruction.opc = TWO_BIT_MASK & (instr >> 29);
    instruction.opi = THREE_BIT_MASK & (instr >> 23);
    instruction.rd = FIVE_BIT_MASK & instr;

    instruction.operand.sh = ONE_BIT_MASK & (instr >> 22);
    instruction.operand.imm12 = TWELVE_BIT_MASK & (instr >> 10);
    instruction.operand.rnOperand = FIVE_BIT_MASK & (instr >> 5);
    instruction.operand.hw = TWO_BIT_MASK & (instr >> 21);
    instruction.operand.imm16 = SIXTEEN_BIT_MASK & (instr >> 5);
}

static void loadDPR(uint32_t instr) {

    instruction.sf = instr >> 31;
    instruction.opc = TWO_BIT_MASK & (instr >> 29);
    instruction.M = ONE_BIT_MASK & (instr >> 28);
    instruction.rm = FIVE_BIT_MASK & (instr >> 16);
    instruction.rnInstruct = FIVE_BIT_MASK & (instr >> 5);
    instruction.rd = FIVE_BIT_MASK & instr;

    instruction.operand.imm6 = SIX_BIT_MASK & (instr >> 10);
    instruction.operand.x = ONE_BIT_MASK & (instr >> 15);
    instruction.operand.ra = FIVE_BIT_MASK & (instr >> 10);
    
    instruction.opr.msb = ONE_BIT_MASK & (instr >> 24);
    instruction.opr.shift = TWO_BIT_MASK & (instr >> 22);
    instruction.opr.N = ONE_BIT_MASK & (instr >> 21);
    instruction.opr.val = FOUR_BIT_MASK & (instr >> 21);
}

static void loadLAS(uint32_t instr) {

    instruction.bit31 = instr >> 31;
    instruction.sf = ONE_BIT_MASK & (instr >> 30);
    instruction.U = ONE_BIT_MASK & (instr >> 24);
    instruction.L = ONE_BIT_MASK & (instr >> 22);
    instruction.xn = FIVE_BIT_MASK & (instr >> 5);
    instruction.rt = FIVE_BIT_MASK & instr;
    instruction.simm19 = signExtend19(NINETEEN_BIT_MASK & (instr >> 5));

    instruction.offset.bit21 = ONE_BIT_MASK & (instr >> 21);
    instruction.offset.xm = FIVE_BIT_MASK & (instr >> 16);
    instruction.offset.simm9 = signExtend9(NINE_BIT_MASK & (instr >> 12));
    instruction.offset.I = ONE_BIT_MASK & (instr >> 11);
    instruction.offset.imm12 = TWELVE_BIT_MASK & (instr >> 10);
}

static void loadB(uint32_t instr) {

    instruction.bits30To31 = TWO_BIT_MASK & (instr >> 30);
    instruction.simm26 = signExtend26(TWENTYSIX_BIT_MASK & instr);
    instruction.xn = FIVE_BIT_MASK & (instr >> 5);
    instruction.simm19 = signExtend19(NINETEEN_BIT_MASK & (instr >> 5));
    instruction.cond = FOUR_BIT_MASK & instr;
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
    printf("%s = %d\n", getName(sf), instruction.sf);
    printf("%s = %d\n", getName(opc), instruction.opc);
    printf("%s = %d\n", getName(opi), instruction.opi);
    printf("%s = %d\n", getName(rd), instruction.rd);
    printf("%s = %d\n", getName(M), instruction.M);
    printf("%s = %d\n", getName(rm), instruction.rm);
    printf("%s = %d\n", getName(rnInstruct), instruction.rnInstruct);
    printf("%s = %d\n", getName(bit31), instruction.bit31);
    printf("%s = %d\n", getName(U), instruction.U);
    printf("%s = %d\n", getName(L), instruction.L);
    printf("%s = %d\n", getName(xn), instruction.xn);
    printf("%s = %d\n", getName(rt), instruction.rt);
    printf("%s = %d\n", getName(cond), instruction.cond);
    printf("%s = %d\n", getName(bits30To31), instruction.bits30To31);
    printf("%s = %ld\n", getName(simm19), instruction.simm19);
    printf("%s = %ld\n", getName(simm26), instruction.simm26);

    printf("Operand.%s = %d\n", getName(sh), instruction.operand.sh);
    printf("Operand.%s = %ld\n", getName(imm12), instruction.operand.imm12);
    printf("Operand.%s = %d\n", getName(rnOperand), instruction.operand.rnOperand);
    printf("Operand.%s = %d\n", getName(hw), instruction.operand.hw);
    printf("Operand.%s = %ld\n", getName(imm16), instruction.operand.imm16);
    printf("Operand.%s = %ld\n", getName(imm6), instruction.operand.imm6);
    printf("Operand.%s = %d\n", getName(x), instruction.operand.x);
    printf("Operand.%s = %d\n", getName(ra), instruction.operand.ra);

    printf("Offset.%s = %d\n", getName(bit21), instruction.offset.bit21);
    printf("Offset.%s = %d\n", getName(xm), instruction.offset.xm);
    printf("Offset.%s = %ld\n", getName(simm9), instruction.offset.simm9);
    printf("Offset.%s = %d\n", getName(I), instruction.offset.I);
    printf("Offset.%s = %ld\n", getName(imm12), instruction.offset.imm12);

    printf("opr.%s = %d\n", getName(msb), instruction.opr.msb);
    printf("opr.%s = %d\n", getName(shift), instruction.opr.shift);
    printf("opr.%s = %d\n", getName(N), instruction.opr.N);
    printf("opr.%s = %d\n", getName(val), instruction.opr.val);
}

// returns a mask ofo the correct bitWidth
uint64_t activeMask(uint8_t bitWidth) {
    return (bitWidth == 32) ? THIRTYTWO_BIT_MASK : SIXTYFOUR_BIT_MASK;
}

// returns the sign bit of a given number
uint8_t signBitOf(uint64_t value, uint8_t bitWidth) {
    return value >> (bitWidth - 1);
}
