#include <stdbool.h>
#include <stdint.h>
#define GENERAL_REG_NUM 31
#define THIRTYTWO_BIT_MASK 0x00000000ffffffff
#define SIXTYFOUR_BIT_MASK 0xffffffffffffffff

// this is to increment the pc to the next instruction if there is no branch
#define NEXT_INSTRUCTION 4

// this is the code that some specific registers have to be to encode the zero register
#define ZERO_REG_CODE 0xf

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

// reading/writing to the correct register mode
extern void writeToRegister(uint64_t, uint64_t, uint8_t);
extern uint64_t readFromRegister(uint8_t, uint8_t);

// initialise function prototype
extern void registerInitialise(void);

// returns the program counter
extern uint64_t getProgramCounter(void);

// returns the zero register
extern uint64_t getZeroRegister(void);

// program counter is incremented by n bytes
extern void incrementProgramCounter(int64_t);

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

// resets the changed variable to false
extern void resetChanged(void);

// returns the value of the changed variable
extern bool getChanged(void);
