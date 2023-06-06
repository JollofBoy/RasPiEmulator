// made headers
#include "dataProcessingRegister.h"
// I am thinking of adding on the dataProcessingImmediate.h because there are useful functions there that I don't wanna repeat
//#include "dataProcessingImmediate.h"
#include "decode.h"
#include "memory.h"
#include "registers.h"

void executeDPR(void) {
    // if the instruction has a shift, then it is performed on Rm instead of Rd
    // sf is the bit-width for all the registers. 0 for 32-bit
}
