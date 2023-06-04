#include <stdbool.h>
#include <stdint.h>
#define GENERAL_REG_NUM 31
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
