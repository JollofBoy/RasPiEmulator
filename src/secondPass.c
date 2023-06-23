// standard headers
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// made headers
#include "secondPass.h"
#include "tokeniser.h"
#include "dataProcessingAsm.h"
#include "branchAsm.h"
#include "loadAndStoreAsm.h"
#include "specialAsm.h"
#include "binFileWriter.h"

// private definitions
#define HALT_INSTRUCTION 0x8a000000

// private functions
static bool same(char *str1, char *str2, int n) {
    return strncmp(str1, str2, n) == 0;
}

// public functions

void secondPassParser(char *asmFile, char *outputFile) {
    // creates a file pointer to a file in read mode
    FILE *fptr = fopen(asmFile, "r");

    if (fptr == NULL) {
       fprintf(stderr, "File Err: can't open %s\n", asmFile);
       exit(1);
    }

    // create the buffer for the line to read
    char buffer[MAX_LINE_LEN];
    
    // create an empty opcode and operand pointer
    char **opcode = malloc(sizeof(char*));
    char **operands = malloc(MAX_OPERANDS * sizeof(char*));
    

    while (fgets(buffer, MAX_LINE_LEN, fptr) != NULL) {

        // gives the number of operands found
        // first we get the instruction is (*opcode)
        int operandCount = tokenise(buffer, opcode, operands);

        // this is the machine code that will be written to the binary file
        uint32_t machineCodeToWrite = 0;

        // we would gloss over the label line as we already have the address of the label
        if (isLabel(*opcode, operandCount)) {
            continue;
        }
        
        // we would use an if-else statement to identify which of the 4 instructions we'd be 
        if (same(*opcode, "add", 3) || same(*opcode, "sub", 3) 
                || same(*opcode, "cm", 2)  || same(*opcode, "neg", 3) 
                || same(*opcode, "and", 3) || same(*opcode, "bic", 3) 
                || same(*opcode, "eo", 2)  || same(*opcode, "or", 2) 
                || same(*opcode, "tst", 3) || same(*opcode, "m", 1)) { /*Data Processing*/
            //TODO go into the data processing file
            printf("Data Processing\n");
            
            // assembles the instruction and stores the machine code
            machineCodeToWrite = dpAssemble(*opcode, operands);

        } else if (same(*opcode, "b", 1)) { /*Branching*/
            //TODO go into the branching file
            printf("Branching\n");

            // assembles the instruction and stores the machine code
            machineCodeToWrite = bAssemble(*opcode, operands);

        } else if (same(*opcode, "str", 3) || same(*opcode, "ldr", 3)) { /*Loads and Stores*/
            //TODO go into the loads and Stores file
            printf("Loads and Stores\n");

            // assembles the instruction and stores the machine code
            machineCodeToWrite = lsAssemble(*opcode, operands);

        } else if (same(*opcode, "nop", 3) || same(*opcode, ".int", 4)){ /*Special*/
            //TODO go into the special file?
            printf("Special\n");

            // assembles the instruction and stores the machine code
            machineCodeToWrite = sAssemble(*opcode, operands);
        }

        // writes the machine code generated from the function into the output file
        writeTo(outputFile, machineCodeToWrite);
        
    }

    // now that everything has been read, output the halt instruction aswell
    writeTo(outputFile, HALT_INSTRUCTION);

}
