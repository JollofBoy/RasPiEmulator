#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "utils.h"

void printBits(uint64_t x, int nbits) {
    uint64_t mask = 1 << (nbits-1);
    for(int i=0; i<nbits; i++) {
        int bit = (x & mask) != 0;
        putchar('0' + bit);
        mask >>= 1;
    }
    putchar('\n');
}

int sizeOfFile(char *filename) {
    // opening the pointer version of the file to read the size of the file
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        fprintf(stderr, "File Err: can't open %s\n", filename);
        exit(1);
    }

    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);

    // closing the pointer for the file
    fclose(fp);

    return size;
}
