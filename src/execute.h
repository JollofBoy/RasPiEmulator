// this is to increment the pc to the next instruction if there is no branch
#define NEXT_INSTRUCTION 4

// does the relevant manipulation of memory and then increments the program counter
extern void executeInstruction(int /* might make this an enum later */);
