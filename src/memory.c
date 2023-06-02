#include <stdint.h>
#include "memory.h"
#include "utils.h"

//#include <stdio.h>

// Here would be where the memory is, it is an array and we'll call it mem
static uint8_t mem[MEM_SIZE];

void memoryInitialise(void) {
    // Initialise mem to have all locations with 0
    for (int i=0; i<MEM_SIZE; i++) {
        mem[i] = 0;
    }
}

// This can read a value from memory
uint32_t memoryRead32(int address) {
    uint32_t value = 0;
    for (int i = address; i < address + FOUR_BYTES; i++) {
        // this looks at each piece in memory and left shifts it by the appropriate number
        value += (0xffffffff & mem[i]) << ((i-address) * BITS);        
    }
    return value;
}

uint64_t memoryRead64(int address) {
    uint64_t value = 0;
    for (int i = address; i < address + EIGHT_BYTES; i++) {
        // this looks at each piece in memory and left shifts it by the appropriate number
        value += (0xffffffffffffffff & mem[i]) << ((i-address) * BITS);        
    }
    return value;
}

// This can write a value to memory
void memoryWrite32(uint32_t value, int address) {
    // here we would put the value into little-endian mode (smallest byte to largest byte)
    for (int i = address; i < address + FOUR_BYTES; i++) {
        mem[i] = 0x000000ff & (value >> ((i-address) * BITS));
    }
}

void memoryWrite64(uint64_t value, int address) {
    // here we would put the value into little-endian mode (smallest byte to largest byte)
    for (int i = address; i < address + EIGHT_BYTES; i++) {
        mem[i] = 0x00000000000000ff & (value >> ((i-address) * BITS));
    }
}

//THIS IS A TEST TING, NOT SERIOUS AT ALL!!!
//int main(void) {
//    memoryInitialise();
//   for (uint32_t j = 0; j < MEM_SIZE/4; j+=4) {
//       memoryWrite32(j/4, j);
//       printf("\nThe number we are representing is: %d\nThe 4 bytes of memory for this is:\n", j /4);
//       printBits(mem[j], 8);
//      printBits(mem[j+1], 8);
//      printBits(mem[j+2], 8);
//      printBits(mem[j+3], 8);
//      printf("%d", memoryRead32(j));
//   }
//   return 0;
//}
