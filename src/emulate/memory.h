#include <stdint.h>
// the maximum size of memory
#define MEM_SIZE 2097152
#define FOUR_BYTES 4
#define EIGHT_BYTES 8
#define BITS 8

// this reads memory from a memory location
extern uint8_t memoryRead8(int);
extern uint32_t memoryRead32(int);
extern uint64_t memoryRead64(int);

// this writes data to a memory location
extern void memoryWrite8(uint8_t, int);
extern void memoryWrite32(uint32_t, int);
extern void memoryWrite64(uint64_t, int);

extern void memoryInitialise(void);

// This prints out the contents of memory
extern void memoryPrintLoadedContents(char*);
