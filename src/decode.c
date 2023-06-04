// standard headers
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// made headers
#include "decode.h"

// private definitions
#define OP0_SHIFT 25
#define OP0_MASK 0x0000000f 
#define ONE_BIT_MASK 0x1
#define TWO_BIT_MASK 0x3 
#define FIVE_BIT_MASK 0x1f

// definining private functions
// DISCLAIMER: there are alot of magic numbers here and those will be sorted out in future improvements TODO

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
    return ptr;
}

// these are static as no other file would need to load anything into memory directly
static void loadDPI(uint32_t id, uint32_t instr) {
    // creates a new stack pointer
    instructionPtr = makeInstructStruct();
    
    // here we are going to literally assign a value to every piece of info relevant
    instructionPtr->sf = instr >> 31;
    instructionPtr->opc = TWO_BIT_MASK & (instr >> 29);
    instructionPtr->opi = 0x7 & (instr >> 23);
    instructionPtr->rd = FIVE_BIT_MASK & instr;

    instructionPtr->operand->sh = ONE_BIT_MASK & (instr >> 22);
    instructionPtr->operand->imm12 = 0xfff & (instr >> 10);
    instructionPtr->operand->rnOperand = FIVE_BIT_MASK & (instr >> 5);
    instructionPtr->operand->hw = TWO_BIT_MASK & (instr >> 21);
    instructionPtr->operand->imm16 = 0xffff & (instr >> 5);
}

static void loadDPR(uint32_t id, uint32_t instr) {
    // creates a new stack pointer
    instructionPtr = makeInstructStruct();

    instructionPtr->sf = instr >> 31;
    instructionPtr->opc = TWO_BIT_MASK & (instr >> 29);
    instructionPtr->M = ONE_BIT_MASK & (instr >> 28);
    instructionPtr->opr = 0xf & (instr >> 21);
    instructionPtr->rm = FIVE_BIT_MASK & (instr >> 16);
    instructionPtr->rnInstruct = FIVE_BIT_MASK & (instr >> 5);
    instructionPtr->rd = FIVE_BIT_MASK & instr;

    instructionPtr->operand->imm6 = 0x3f & (instr >> 10);
    instructionPtr->operand->x = ONE_BIT_MASK & (instr >> 15);
    instructionPtr->operand->ra = FIVE_BIT_MASK & (instr >> 10);
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
    instructionPtr->simm19 = 0x3ffff & (instr >> 5);

    instructionPtr->offset->bit21 = instr >> 21;
    instructionPtr->offset->xm = FIVE_BIT_MASK & (instr >> 16);
    instructionPtr->offset->simm9 = 0x1ff & (instr >> 12);
    instructionPtr->offset->I = ONE_BIT_MASK & (instr >> 11);
    instructionPtr->offset->imm12 = 0xfff & (instr >> 10);
}

static void loadB(uint32_t id, uint32_t instr) {
    // creates a new stack pointer
    instructionPtr = makeInstructStruct();

}

// under here include a global variable pointer that will point to the instruction_t struct
instruction_t *instructionPtr;

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

void deleteInstructStruct(instruction_t *ptr) {
    // the if is to prevent freeing an already freed pointer
    if (ptr != NULL) {
        free(ptr->operand);
        free(ptr->offset);
        free(ptr);
    }
}
