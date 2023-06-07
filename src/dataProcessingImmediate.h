#include <stdint.h>

// executes a data processing immediate instruction
extern void executeDPI(void);

// masks the value to the appropriate size
extern uint64_t activeMask(uint8_t);
extern uint64_t activeMax(uint8_t);

// finds the sign bit of a number of a certain size
extern uint8_t signBitOf(uint64_t, uint8_t);

// writes a value to the register of appropriate size
extern void writeToRegister(uint64_t, uint64_t, uint8_t);

// used to find the result of an arithmetic operation
extern uint64_t arithOpOn(uint64_t, uint64_t, uint8_t, uint8_t);
