#include "trie.h"

#include <stdio.h>
#include <stdlib.h>

#define EMPTY_CODE 1

//Creates a trie_node with t->code as code.
TrieNode *trie_node_create(uint16_t code) {
    TrieNode *t = (TrieNode *) calloc(1, sizeof(TrieNode));
    if (t) {

        t->code = code;
        return t;
    }
    return t;
}

//Frees memory for a node.
void trie_node_delete(TrieNode *n) {
    free(n);
    n = NULL;
}

//Creates a trie, consisting of a root and a children array all of
//Null pointers.
TrieNode *trie_create(void) {
    return trie_node_create(EMPTY_CODE);
}

//Resets a trie to its root state, all root->children are freed.
void trie_reset(TrieNode *root) {
    for (int i = 0; i < ALPHABET; i++) {
        trie_delete(root->children[i]);
        root->children[i] = NULL;
    }
}

//Deletes a trie, including the root.
void trie_delete(TrieNode *n) {
    if (n != NULL) {
        for (int i = 0; i < ALPHABET; i++) {
            trie_delete(n->children[i]);
        }
    }

    trie_node_delete(n);
}

//Returns the child at sym index of a node.
TrieNode *trie_step(TrieNode *n, uint8_t sym) {
    return n->children[sym];
}
