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
#include "outputting.h"
#include "utils.h"

// private definitions
#define HALT_INSTRUCTION 0x8a000000
#define NO_OP_INSTRUCTION 0xd503201f

int main(int argc, char **argv) {
  
    // these are too many inputs or too little inputs
    if (argc < 2) {
        fprintf(stderr, "Usage: too little inputs. Give 2\n");
        exit(1);
    } else if (argc > 3) {
        fprintf(stderr, "Usage: too many inputs. Give 2\n");
        exit(1);
    }

    // this loads the values from the binary file into memory
    char *binFile = argv[1];
    load(binFile);
    
    // this is where the emulator while loop is ran
    run();

    // we output the relevant values to the output file that is parsed
    if (argc == 3) {
        char *outFile = argv[2];
        output(outFile);
    }
    return EXIT_SUCCESS;
}

void run(void) {
    // this is where the main while loop is
    bool running = true;
    while (running) {

        // FETCH the instruction
        uint32_t fetchedInstruction = fetchInstruction();
        
        // DECODE and EXECUTE the special instructions
        if (fetchedInstruction == HALT_INSTRUCTION) {
            running = false;
            break;
        } else if (fetchedInstruction == NO_OP_INSTRUCTION) {
            // EXECUTE the no operation instruction (-1 would mean to skip in the executeInstruction function)
            executeInstruction(-1);
        } else {
            // DECODES a non-special instruction
            group_t instructionGroup = decodeInstruction(fetchedInstruction);

            // EXECUTE the instruction
            executeInstruction(instructionGroup);
        }
    }
}
