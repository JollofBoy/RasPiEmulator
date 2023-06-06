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

// definining private functions
// DISCLAIMER: there are alot of magic numbers here and those will be sorted out in future improvements TODO

// under here include a global variable pointer that will point to the instruction_t struct
instruction_t *instructionPtr;

// this creates a pointer to the instruction_t struct
static instruction_t *makeInstructStruct(void) {
    instruction_t *ptr = malloc(sizeof(instruction_t));
    if (ptr == NULL) {
        return NULL;
    }
    ptr->operand = malloc(sizeof(operand_t));
    if (ptr->operand == NULL) {
        free(ptr);
        return NULL;
    }
    ptr->offset = malloc(sizeof(offset_t));
    if (ptr->offset == NULL) {
        free(ptr->operand);
        free(ptr);
        return NULL;
    }

    ptr->opr = malloc(sizeof(opr_t));
    if (ptr->opr == NULL) {
        free(ptr->operand);
        free(ptr->offset);
        free(ptr);
        return NULL;
    }

    return ptr;
}

// these are static as no other file would need to load anything into memory directly
static void loadDPI(uint32_t id, uint32_t instr) {
    // creates a new stack pointer
    instructionPtr = makeInstructStruct();
    
    // here we are going to literally assign a value to every piece of info relevant
    instructionPtr->sf = instr >> 31;
    instructionPtr->opc = TWO_BIT_MASK & (instr >> 29);
    instructionPtr->opi = THREE_BIT_MASK & (instr >> 23);
    instructionPtr->rd = FIVE_BIT_MASK & instr;

    instructionPtr->operand->sh = ONE_BIT_MASK & (instr >> 22);
    instructionPtr->operand->imm12 = TWELVE_BIT_MASK & (instr >> 10);
    instructionPtr->operand->rnOperand = FIVE_BIT_MASK & (instr >> 5);
    instructionPtr->operand->hw = TWO_BIT_MASK & (instr >> 21);
    instructionPtr->operand->imm16 = SIXTEEN_BIT_MASK & (instr >> 5);
}

static void loadDPR(uint32_t id, uint32_t instr) {
    // creates a new stack pointer
    instructionPtr = makeInstructStruct();

    instructionPtr->sf = instr >> 31;
    instructionPtr->opc = TWO_BIT_MASK & (instr >> 29);
    instructionPtr->M = ONE_BIT_MASK & (instr >> 28);
    instructionPtr->rm = FIVE_BIT_MASK & (instr >> 16);
    instructionPtr->rnInstruct = FIVE_BIT_MASK & (instr >> 5);
    instructionPtr->rd = FIVE_BIT_MASK & instr;

    instructionPtr->operand->imm6 = SIX_BIT_MASK & (instr >> 10);
    instructionPtr->operand->x = ONE_BIT_MASK & (instr >> 15);
    instructionPtr->operand->ra = FIVE_BIT_MASK & (instr >> 10);
    
    instructionPtr->opr->msb = ONE_BIT_MASK & (instr >> 24);
    instructionPtr->opr->shift = TWO_BIT_MASK & (instr >> 22);
    instructionPtr->opr->N = ONE_BIT_MASK & (instr >> 21);
}

static void loadLAS(uint32_t id, uint32_t instr) {
    // creates a new stack pointer
    instructionPtr = makeInstructStruct();

    instructionPtr->bit31 = instr >> 31;
    instructionPtr->sf = ONE_BIT_MASK & (instr >> 30);
    instructionPtr->U = ONE_BIT_MASK & (instr >> 24);
    instructionPtr->L = ONE_BIT_MASK & (instr >> 22);
    instructionPtr->xn = FIVE_BIT_MASK & (instr >> 5);
    instructionPtr->rt = FIVE_BIT_MASK & instr;
    instructionPtr->simm19 = NINETEEN_BIT_MASK & (instr >> 5);

    instructionPtr->offset->bit21 = instr >> 21;
    instructionPtr->offset->xm = FIVE_BIT_MASK & (instr >> 16);
    instructionPtr->offset->simm9 = NINE_BIT_MASK & (instr >> 12);
    instructionPtr->offset->I = ONE_BIT_MASK & (instr >> 11);
    instructionPtr->offset->imm12 = TWELVE_BIT_MASK & (instr >> 10);
}

static void loadB(uint32_t id, uint32_t instr) {
    // creates a new stack pointer
    instructionPtr = makeInstructStruct();

    instructionPtr->bits30To31 = TWO_BIT_MASK & (instr >> 30);
    instructionPtr->simm26 = TWENTYSIX_BIT_MASK & instr;
    instructionPtr->xn = FIVE_BIT_MASK & (instr >> 5);
    instructionPtr->simm19 = NINETEEN_BIT_MASK & (instr >> 5);
    instructionPtr->cond = FOUR_BIT_MASK & instr;
}

// this is the identifier value to see what group the instruction goes under
static uint32_t op0; 

group_t decodeInstruction(uint32_t instruction) {
    group_t group;
    // we set the op0 to its value
    op0 = OP0_MASK & (instruction >> OP0_SHIFT);

    // finds what the group of the instruction is based on the op0 and then loads values to the relevant markers
    switch (op0) {
        case 0x8:
        case 0x9:
            group = DP_IMMEDIATE;
            printf("DP IMMEDIATE\n");
            loadDPI(op0, instruction); // this sets the structs to every possible value in the case of DP_IMMEDIATE
            break;
        case 0x5:
        case 0xd:
            group = DP_REGISTER;
            printf("DP REGISTER\n");
            loadDPR(op0, instruction); // this sets the structs to every possible value in the case of DP_REGISTER
            break;
        case 0x4:
        case 0x6:
        case 0xc:
        case 0xe:
            group = LOADS_AND_STORES;
            printf("LOADS AND STORES\n");
            loadLAS(op0, instruction); // this sets the structs to every possible value in the case of LOADS_AND_STORES
            break;
        case 0xa:
        case 0xb:
            group = BRANCHES;
            printf("BRANCHES\n");
            loadB(op0, instruction); // this sets the structs to every possible value in the case of BRANCHES
            break;
        default:
            printf("NO MATCH. op0 = %x\n", op0);
    }

    return group;
}

void printInstructStructContents(void) {
    printf("%s = %x\n", getName(sf), instructionPtr->sf);
    printf("%s = %x\n", getName(opc), instructionPtr->opc);
    printf("%s = %x\n", getName(opi), instructionPtr->opi);
    printf("%s = %x\n", getName(rd), instructionPtr->rd);
    printf("%s = %x\n", getName(M), instructionPtr->M);
    printf("%s = %x\n", getName(rm), instructionPtr->rm);
    printf("%s = %x\n", getName(rnInstruct), instructionPtr->rnInstruct);
    printf("%s = %x\n", getName(bit31), instructionPtr->bit31);
    printf("%s = %x\n", getName(U), instructionPtr->U);
    printf("%s = %x\n", getName(L), instructionPtr->L);
    printf("%s = %x\n", getName(xn), instructionPtr->xn);
    printf("%s = %x\n", getName(rt), instructionPtr->rt);
    printf("%s = %x\n", getName(cond), instructionPtr->cond);
    printf("%s = %x\n", getName(bits30To31), instructionPtr->bits30To31);

    printf("%s = %x\n", getName(simm19), instructionPtr->simm19);
    printf("%s = %x\n", getName(simm26), instructionPtr->simm26);

    printf("Operand -> %s = %d\n", getName(sh), instructionPtr->operand->sh);
    printf("Operand -> %s = %d\n", getName(imm12), instructionPtr->operand->imm12);
    printf("Operand -> %s = %d\n", getName(rnOperand), instructionPtr->operand->rnOperand);
    printf("Operand -> %s = %d\n", getName(hw), instructionPtr->operand->hw);
    printf("Operand -> %s = %d\n", getName(imm16), instructionPtr->operand->imm16);
    printf("Operand -> %s = %d\n", getName(imm6), instructionPtr->operand->imm6);
    printf("Operand -> %s = %d\n", getName(x), instructionPtr->operand->x);
    printf("Operand -> %s = %d\n", getName(ra), instructionPtr->operand->ra);
    
    printf("Offset -> %s = %d\n", getName(bit21), instructionPtr->offset->bit21);
    printf("Offset -> %s = %d\n", getName(xm), instructionPtr->offset->xm);
    printf("Offset -> %s = %d\n", getName(simm9), instructionPtr->offset->simm9);
    printf("Offset -> %s = %d\n", getName(I), instructionPtr->offset->I);
    printf("Offset -> %s = %d\n", getName(bit10), instructionPtr->offset->bit10);
    printf("Offset -> %s = %d\n", getName(imm12), instructionPtr->offset->imm12);
   
    printf("opr -> %s = %d\n", getName(msb), instructionPtr->opr->msb);
    printf("opr -> %s = %d\n", getName(shift), instructionPtr->opr->shift);
    printf("opr -> %s = %d\n", getName(N), instructionPtr->opr->N);
}

void deleteInstructStruct(instruction_t *ptr) {
    // the if is to prevent freeing an already freed pointer
    if (ptr != NULL) {
        free(ptr->operand);
        free(ptr->offset);
        free(ptr->opr);
        free(ptr);
    }
}
