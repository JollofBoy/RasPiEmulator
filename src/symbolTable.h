#include <stdint.h>
#include <stdbool.h>

// a macro that makes the binary search tree null
#define makeEmptyBst() NULL 

// making the structure for a binary search tree
struct bst {
    char *key;
    uint64_t value;
    struct bst *left;
    struct bst *right;
};

// the type of this ADT is bst now
typedef struct bst *bst;

extern void addToBst(char*, uint64_t);
extern bool lookup(char*, uint64_t*);
extern void freeBst(bst);
