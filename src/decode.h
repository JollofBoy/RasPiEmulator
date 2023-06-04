#include <stdint.h>

// an enum marking each type of instruction for execute to use
typedef enum {DP_IMMEDIATE, DP_REGISTER, LOADS_AND_STORES, BRANCHES} group_t;

// a struct that contains every possible operand layout
typedef struct {
    uint32_t sh;
    uint32_t imm12;
    uint32_t rnOperand;
    uint32_t hw;
    uint32_t imm16;
    //TODO here we will work with the opr/opc/N related stuff later
    uint32_t imm6;
    uint32_t x;
    uint32_t ra;
} operand_t;

// a struct that contains every possible offset layout
typedef struct {
    uint32_t bit21;
    uint32_t xm;
    int32_t simm9;
    uint32_t I;
    uint32_t bit10;
    uint32_t imm12;
    //TODO might create some more members to better represent the offset
} offset_t;

// making a struct which contains every possible memory layout, where operand, opr, and offset are their own structures as well
typedef struct {
    uint32_t sf;
    uint32_t opc;
    uint32_t opi;
    operand_t *operand;    
    uint32_t rd;
    uint32_t M;
    uint32_t opr;
    uint32_t rm;
    uint32_t rnInstruct;
    uint32_t bit31;
    uint32_t U;
    uint32_t L;
    offset_t *offset;
    uint32_t xn;
    uint32_t rt;
    int32_t simm19;
    int32_t simm26;
    uint32_t cond; //TODO might make this an enum with the mnemonic and the equivalent code (makes things easier here I think)
    uint32_t bits30To31;
} instruction_t;

// under here include a global variable pointer that will point to the instruction_t struct
extern instruction_t *instructionPtr;
    
// decodes an instruction and sets the structs to contain every possible marker for the given op0
extern group_t decodeInstruction(uint32_t);

// frees the pointer
extern void deleteInstructStruct(instruction_t *ptr);
