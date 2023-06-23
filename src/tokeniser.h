// standard headers
#include <stdbool.h>
#include <stdint.h>

// breaks the line into its parts
extern int tokenise(char *, char **, char **);

// breaks the register into its mode and value
extern int8_t decomposeRegister(char*, char**);

// returns true if it is the type mentioned
extern bool isLabel(char*, int);
extern bool isReg(char*);
extern bool isImm(char*);
