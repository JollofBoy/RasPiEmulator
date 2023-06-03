#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "emulate.h"
#include "memory.h"
#include "registers.h"
#include "fetch.h"
#include "decode.h"
#include "execute.h"
#include "utils.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: input a binary file!\n");
        exit(1);
    }

    // this initialises the registers and memory
    memoryInitialise();
    registerInitialise();

    // this loads the values from the binary file into memory
    load(argv[1]);
    
    // this is where the emulator while loop is held
    run();
    return EXIT_SUCCESS;
}

void load(char *filename) {
    // opening the file
    FILE *fptr = fopen(filename, "rb");

    if (fptr == NULL) {
        fprintf(stderr, "File Err: can't open %s\n", filename);
        exit(1);
    }
   
    // loading the contents of the file into memory
    unsigned int ch;
    int j = 0;
    while ((ch = getc(fptr)) != EOF) {
        memoryWrite8(((char) ch) & 0xff, j);
        j++;
    }

    // close the file
    fclose(fptr);

    // can optionally print out the contents of main memory
    //memoryPrintLoadedContents(filename);
    return;
}

void run(void) {
    // this is where the main while loop is
    bool running = true;
    while (running) {

        // FETCH the instruction
        uint32_t fetchedInstruction = memoryRead32(programCounter);
        // the program counter increments here for now
        programCounter += 1;

        // DECODE the instruction
        // In it's corresponding file, we will have an instruction set of enumerated types so that we can correctly execute
        if (fetchedInstruction == 0x8a000000) {
            running = false;
            break;
        }

        // EXECUTE the instruction
    }
    return;
}
