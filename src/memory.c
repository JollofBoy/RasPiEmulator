#include <stdint.h>
#include <stdio.h>
#include "memory.h"
#include "utils.h"

// Here would be where the memory is, it is an array and we'll call it mem
static uint8_t mem[MEM_SIZE];

void memoryInitialise(void) {
    // Initialise mem to have all locations with 0
    for (int i=0; i<MEM_SIZE; i++) {
        mem[i] = 0;
    }
}

// This can read a value from memory
uint8_t memoryRead8(int address) {
    return mem[address];
}

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
void memoryWrite8(uint8_t value, int address) {
    mem[address] = value;
}

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

void memoryPrintLoadedContents(char *filename) {
    // store the file size
    int size = sizeOfFile(filename);

    printf("The size of this file is: %d bytes\n", size);

        // printing out the contents of the memory
        for (int address=0; address<size; address++) {
            printf("The value of address: %x is :", address);
            printf("%x\n", memoryRead8(address));
        }

        // prints out the words in memory location
        for (int a=0; a<size; a+=4) {
            printf("The word at address %x is: %x\n", a, memoryRead32(a));
        }
}

// down here we will include printing the loaded contents of the memory later. Also we will tweak the way in which we print for 
// it to go to an out file
