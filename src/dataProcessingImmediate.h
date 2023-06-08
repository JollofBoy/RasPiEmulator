#include <stdint.h>

// executes a data processing immediate instruction
extern void executeDPI(void);

// used to find the result of an arithmetic operation
extern uint64_t arithOpOn(uint64_t, uint64_t, uint8_t, uint8_t);
