#include <stdint.h>

// definitions for known bit masks
#define ONE_BIT_MASK 0x1
#define TWO_BIT_MASK 0x3 
#define THREE_BIT_MASK 0x7 
#define FOUR_BIT_MASK 0xf
#define FIVE_BIT_MASK 0x1f
#define SIX_BIT_MASK 0x3f 
#define NINE_BIT_MASK 0x1ff
#define TWELVE_BIT_MASK 0xfff 
#define SIXTEEN_BIT_MASK 0xffff
#define NINETEEN_BIT_MASK 0x3ffff 
#define TWENTYSIX_BIT_MASK 0x3ffffff
#define THIRTYTWO_BIT_MASK 0x00000000ffffffff
#define SIXTYFOUR_BIT_MASK 0xffffffffffffffff

// definitions to sign extend the simm values to 64bits
#define NINE_SIGN_EXTENSION_MASK 0xfffffffffffffe00
#define NINETEEN_SIGN_EXTENSION_MASK 0xfffffffffff80000
#define TWENTYSIX_SIGN_EXTENSION_MASK 0xfffffffffc000000

// definition for the minimum unsigned number
#define MIN 0

// an enum marking each type of instruction for execute to use
typedef enum {DP_IMMEDIATE, DP_REGISTER, LOADS_AND_STORES, BRANCHES} group_t;

// a struct that contains every possible operand layout
typedef struct {
    uint8_t sh;
    uint64_t imm12;
    uint8_t rnOperand;
    uint8_t hw;
    uint64_t imm16;
    uint64_t imm6;
    uint8_t x;
    uint8_t ra;
} operand_t;

// a struct that contains every possible offset layout
typedef struct {
    uint8_t bit21;
    uint8_t xm;
    int64_t simm9;
    uint8_t I;
    uint64_t imm12;
} offset_t;

// a struct that contains every possible opr layout
typedef struct {
    uint8_t msb;
    uint8_t shift;
    uint8_t N;
    uint8_t val;
} opr_t;

// making a struct which contains every possible memory layout, where operand, opr,
// and offset are their own structures as well
typedef struct {
    uint8_t sf;
    uint8_t opc;
    uint8_t opi;
    operand_t operand;    
    uint8_t rd; 
    uint8_t M;
    opr_t opr;
    uint8_t rm;
    uint8_t rnInstruct;
    uint8_t bit31;
    uint8_t U;
    uint8_t L;
    offset_t offset;
    uint8_t xn;
    uint8_t rt;
    int64_t simm19;
    int64_t simm26;
    uint8_t cond;     
    uint8_t bits30To31;
} instruction_t;

// under here include a global variable pointer that will point to the instruction_t struct
extern instruction_t instruction;
    
// decodes an instruction and sets the structs to contain every possible marker for the given op0
extern group_t decodeInstruction(uint32_t);

extern void printInstructStructContents(void);

// masks the value to the appropriate size
extern uint64_t activeMask(uint8_t);

// finds the sign bit of a number of a certain size
extern uint8_t signBitOf(uint64_t, uint8_t);
