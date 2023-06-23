// standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// made headers
#include "binFileWriter.h"

// private definitions
#define ONE_BYTE_MASK 0xff
#define NEXT_BYTE 8

void writeTo(char *outputFile, uint32_t mCode) {
    // create a file pointer to the output file
    FILE *fptr = fopen(outputFile, "wb");
    char byte = '\0';

    if (fptr == NULL) {
       fprintf(stderr, "File Err: can't create %s\n", outputFile);
       exit(1);
    }

    for (int i = 0; i < 4; i++) {
        byte = (mCode >> (NEXT_BYTE * i)) & ONE_BYTE_MASK;
        // this will have the next byte to have
        fprintf(fptr, "%c", byte);
    }

    // close the file once done
    fclose(fptr);
}
