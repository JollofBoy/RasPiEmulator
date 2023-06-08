// standard headers
#include <stdio.h>
#include <stdlib.h>

// made headers
#include "setup.h"
#include "memory.h"
#include "registers.h"

void initialise(void) {
    // this initialises the registers and memory
    memoryInitialise();
    registerInitialise();
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
