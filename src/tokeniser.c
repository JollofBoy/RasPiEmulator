// standard headers
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

// made headers
#include "tokeniser.h"

// private functions

// public functions

// returns true for if the opcode is a label or not
bool isLabel(char *opc, int operandCount) {
    if (operandCount != 0) {
        return false;
    } else if (opc[strlen(opc) - 2] == ':' ) { /*the word is a label so we note the address */
        return true;
    } else {
        return false;
    }
}

// returns true if the opcode is a register
bool isReg(char *opc) {
    return opc[0] == 'w' || opc[0] == 'x';
}

// returns true if the opcode is an immediate value
bool isImm(char *opc) {
    return opc[0] == '#';
}

// returns the integer value of a register and changes the mode to a w or x to signify 32 or 64 bit
int8_t decomposeRegister(char *reg, char **mode) {
    char *newReg = strdup(reg);
    assert(newReg != NULL);

    // updates the mode to a w or x and removes the non-integer part of the register
    strcpy(*mode, newReg);
    newReg++;
    
    // returns the value of the register if it is a regular register, otherwise, -1 is returned
    // TODO may have to change this later on for non zero register things
    if (isdigit(*newReg)) {
        int8_t registerNum = atoi(newReg);
        return registerNum;
    } else { /*this is a special register*/
        return -1;
    }
}

// breaks a line given into its opcode and operands by updating the opcode and operand fields
// this also returns the number of operands after tokenising the line
int tokenise(char *instruction, char **opcode, char **operands) {
    char *token = NULL; // this will hold the tokens of the line
    char *saveptr = NULL;
    const char delimiters[] = " ,";

    // duplicate the instruction for now in case of later manipulations
    char *dupInstr = strdup(instruction); 
    assert(dupInstr != NULL); // should be able to duplicate the instruction
    
    // takes the first word of line
    token = strtok_r(dupInstr, delimiters, &saveptr);
    *opcode = strdup(token);
    assert(*opcode != NULL); // should be able to duplicate the opcode

    // counts the number of operands in the instruction
    int operandCount = 0;
    while (token != NULL) {
        // first, remove any trailing whitespace
        while (isspace((unsigned char) *dupInstr)) dupInstr++;

        // grabs the next operand
        token = strtok_r(NULL, delimiters, &saveptr);

        // if the token is empty, then end the program
        if (token == NULL) break;

        // copies the duplicated token to the operand array
        operands[operandCount] = strdup(token);
        assert(operands[operandCount] != NULL);

        // the number of operands has now gone up by 1
        operandCount++;
    }

    // returns the number of operands counted
    return operandCount;
}
