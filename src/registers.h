#include <stdbool.h>
#include <stdint.h>

#define GENERAL_REG_NUM 31
#define THIRTYTWO_BIT_MASK 0x00000000ffffffff
#define SIXTYFOUR_BIT_MASK 0xffffffffffffffff

// This contains the types of registers
typedef struct {
    bool negativeFlag;
    bool zeroFlag;
    bool carryFlag;
    bool overflowFlag;
} pstate_t;

// 64-bit manipulation
extern uint64_t readXn(int);
extern void writeXn(uint64_t, int);

// 32-bit manipulation
extern uint32_t readWn(int);
extern void writeWn(uint32_t, int);

// initialise function prototype
extern void registerInitialise(void);

// returns the program counter
extern uint64_t getProgramCounter(void);

// program counter is incremented by n bytes
extern void incrementProgramCounter(int);

// these set the flag value
extern void setN(bool);
extern void setZ(bool);
extern void setC(bool);
extern void setV(bool);

// these return the value of their respective flag
extern bool getN(void);
extern bool getZ(void);
extern bool getC(void);
extern bool getV(void);
