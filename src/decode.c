// standard headers
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// made headers
#include "decode.h"

// private definitions
#define OP0_SHIFT 25
#define OP0_MASK 0x0000000f 

// definining private functions

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
static void loadDPI(uint32_t id) {
    instructionPtr = makeInstructStruct();
}

static void loadDPR(uint32_t id) {
    instructionPtr = makeInstructStruct();
}

static void loadLAS(uint32_t id) {
    instructionPtr = makeInstructStruct();
}

static void loadB(uint32_t id) {
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
            loadDPI(op0); // this sets the structs to every possible value in the case of DP_IMMEDIATE
            break;
        case 0x5:
        case 0xd:
            group = DP_REGISTER;
            printf("DP REGISTER\n");
            loadDPR(op0); // this sets the structs to every possible value in the case of DP_IMMEDIATE
            break;
        case 0x4:
        case 0x6:
        case 0xc:
        case 0xe:
            group = LOADS_AND_STORES;
            printf("LOADS AND STORES\n");
            loadLAS(op0); // this sets the structs to every possible value in the case of DP_IMMEDIATE
            break;
        case 0xa:
        case 0xb:
            group = BRANCHES;
            printf("BRANCHES\n");
            loadB(op0); // this sets the structs to every possible value in the case of DP_IMMEDIATE
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
