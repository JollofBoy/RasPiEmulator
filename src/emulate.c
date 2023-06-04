// standard headers
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

// made headers
#include "emulate.h"
#include "setup.h"
#include "fetch.h"
#include "decode.h"
#include "execute.h"
#include "utils.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: input a binary file!\n");
        exit(1);
    }


    // this loads the values from the binary file into memory
    load(argv[1]);
    
    // this is where the emulator while loop is held
    run();
    // at this point the loop has ended so we output the contents of the register state
    return EXIT_SUCCESS;
}

void run(void) {
    // this is where the main while loop is
    bool running = true;
    while (running) {

        // FETCH the instruction
        uint32_t fetchedInstruction = fetchInstruction();
        
        // PRINTING THE FETCHED INSTRUCTION FOR TESTING
        printf("the fetched instruction is: %x\n", fetchedInstruction);

        // DECODE the instruction
        // In it's corresponding file, we will have an instruction set of enumerated types so that we can correctly execute
        if (fetchedInstruction == 0x8a000000) {
            running = false;
            break;
        }
        // this decodes a non-special instruction
        group_t instructionGroup = decodeInstruction(fetchedInstruction);

        // EXECUTE the instruction
        executeInstruction(instructionGroup);
    }
}
