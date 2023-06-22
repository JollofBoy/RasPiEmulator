// standard headers
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
    if (argc != 3) { /* there must be 2 inputs */
        fprintf(stderr, "Usage: Write ./emulate <file_in> <file_out>\n");
        exit(1);
    }

    // contains the location of the assembly and binary files
    char *asmFile = argv[1];
    char *binFile = argv[2];

    // opens and makes a token for each instruction and whatnot

    // TODO I think I will just pass everything into a different file where it will be using the
    // while loop
    //
    // so first we would identify the string of text, then break it down. I am assuming we will use
    // the 2 pass technique so we'll see

  return EXIT_SUCCESS;
}

/* THIS IS HOW TO WRITE THE ASSEMBLER, SO THAT I CAN CODE IT QUICKLY
 
 * ADDRESSES AND OFFSETS Branch and Load instructions can have a literal address argument
  (either a label/unsigned immediate :preceded by a # symbol) 
 
 * Labels - strings that start with a letter, underscore/period, followed by alphanumeric characters, dollar signs, underscores and/or periods
 
 * the REGEX for labels is [a-zA-Z_\.]([a-zA-Z$_\.])*  
 
 * TODO so what I am going to do is create a tokeniser and then find whatever the leading instruction is, if it belongs to one of the categories, then we work on it
 
 * TODO after we have made this, I will make a file which will output the instruction as a bunch of bytes in the bin file for each category so that we can test 
 * incrementally
 
 * TODO once this has been completed, we will work on each instruction piece by piece and then it should all work god willing
 
 * HOW DOES A TWO PASS ASSEMBLER WORK?
 
 * so make a pass1 and a pass2 file perhaps
 *  TODO make a symbol and a literal table with pass 1 (Analysis)
