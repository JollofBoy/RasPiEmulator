// standard headers
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// made headers
#include "firstPass.h"
#include "symbolTable.h"
#include "tokeniser.h"

// public functions

void firstPassParser (char *filename) {
    // creates a file pointer to a file in read mode
    FILE *fptr = fopen(filename, "r");

    if (fptr == NULL) {
       fprintf(stderr, "File Err: can't open %s\n", filename);
       exit(1);
    }

    // create the buffer for the line to read
    char buffer[MAX_LINE_LEN];

    // create an empty opcode and operand pointer
    char **opcode = malloc(sizeof(char*));
    char **operands = malloc(MAX_OPERANDS * sizeof(char*));

    // create the memory address counter to keep track of the address of the instruction
    uint64_t memAddrCounter = 0;

    // the loop goes over the document line by line and does some string manipulation to the line 
    // to find if there is a potential label or not
    // while the line that is read is not empty
    while (fgets(buffer, MAX_LINE_LEN, fptr) != NULL) {

        // gives the number of operands found
        int operandCount = tokenise(buffer, opcode, operands);
        int opcodeLen = strlen(*opcode);

        if (isLabel(*opcode, operandCount)) { /*the word is a label so we note the address */
            char *label = strdup(*opcode);
            label[opcodeLen-2] = '\0'; // removes the ':' from the label
                                       
            // TODO TEST
            printf("LABEL DETECTED: %s with address %ld\n", label, memAddrCounter);
 
            // adds the label to the symbol table
            addToBst(label, memAddrCounter);
        }

        // TODO may have to fix the logic for this part
        // increment the memory address counter to the next instruction
        memAddrCounter += 4;
    }

    // closes the file that was opened
    fclose(fptr);
}
