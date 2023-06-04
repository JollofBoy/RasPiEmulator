// standard headers
#include <stdint.h>
#include "fetch.h"

// made headers
#include "memory.h"
#include "registers.h"

uint32_t fetchInstruction(void) {
    // Here we would need to access the memory in some way 
    return memoryRead32(getProgramCounter());
}
