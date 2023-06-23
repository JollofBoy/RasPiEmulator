// standard headers
#include <stdlib.h>
#include <stdio.h>

// made headers
#include "assemble.h"
#include "firstPass.h"
#include "secondPass.h"

int main(int argc, char **argv) {
    if (argc != 3) { /* there must be 2 inputs */
        fprintf(stderr, "Usage: Write ./emulate <file_in> <file_out>\n");
        exit(1);
    }

    // contains the location of the assembly and binary files
    char *asmFile = argv[1];
    char *binFile = argv[2];

    // adds the contents of the file to a symbol table that will contain the addresses of the 
    // labels that are looked at
    firstPassParser(asmFile);

    // performs a second pass on the file and writes the machine code to the binary file
    secondPassParser(asmFile, binFile);

    // TODO I think I will just pass everything into a different file where it will be using the
    // while loop
    //
    // so first we would identify the string of text, then break it down. I am assuming we will use
    // the 2 pass technique so we'll see

  return EXIT_SUCCESS;
}

/* TODO after we have made this, I will make a file which will output the instruction as a bunch of bytes in the bin file for each category so that we can test 
 * incrementally
 
 * TODO once this has been completed, we will work on each instruction piece by piece and then it should all work god willing
 
 *  TODO make a symbol and a literal table with pass 1 (Analysis)
 */
