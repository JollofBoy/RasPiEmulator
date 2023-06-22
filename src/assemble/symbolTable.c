// standard headers
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

// made headers
#include "symbolTable.h"

// making a private symbolTable that can only be accessed by the 
static bst symbolTable = makeEmptyBst();

// Private functions

//making a new bst node with <key>, duplicating the key 
// Abort if any memory allocation fails;
static bst makeNode(char *key, uint64_t value) {
    bst newT = malloc(sizeof(struct bst));
    assert(newT != NULL);
    
    char *dupKey = strdup(key);
    assert(dupKey != NULL); 

    newT->key = dupKey;
    newT->value = value;

    newT->left = NULL;
    newT->right = NULL;

    return newT;
}

// the root node is parsed to then check if it's empty first. if not, then the tree is traversed
// according to the key's value
static bst addNode(bst rootNode, char *key, uint64_t value) {
    if (rootNode == NULL) { /*the node is empty*/
        return makenode(key,value);
    }
    
    int cmp = strcmp(key, rootNode->key); 
    if (cmp == 0) { /*the value for that key is overwitten*/
        return rootNode;
    } else if (cmp < 0) { /*we add to the left child*/
        rootNode->left = addNode(rootNode->left, key, value);
    } else { /*we add to the right child*/
        rootNode->right = addNode(rootNode->right, key, value);
    }
    // we return the new state of the tree
    return rootNode;
}

// returns true and updates the value parsed to the value of that key if it exists in the tree
// otherwise, false is returned
static bool lookupHelper(bst rootNode, char *key, uint64_t *val) {
    // if the tree has been fully searched through, then we return false
    if (rootNode == NULL) {
        return false;
    }

    int cmp = strcmp(key, rootNode->key);
    if (cmp == 0) { /*we have found the key*/
        *val = rootNode->value;
        return true;
    } else if (cmp < 0) { /*we look into the left node*/ 
        return lookupHelper(rootNode->left, key, val);
    } else { /*we look into the right node*/
        return lookupHelper(rootNode->right, key, val);
    }
}


// Public functions

void addToBst(char *key, uint64_t value) {
    symbolTable = addNode(symbolTable, key, value);
}

bool lookup(char *key, uint64_t *val) {
    return lookupHelper(symbolTable, key, val);
} 

// recursively frees the binary search tree
void freeBst(bst rootNode) {
    if (rootNode == NULL) { /*don't have any child nodes to free*/
        free(rootNode);
    } else { /*needs to free the left and right nodes before freeing itself*/
        freeBst(rootNode->left);
        freeBst(rootNode->right);
        free(rootNode);
    }
   }
