#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Creates a copy of a string by copying len elements from a
//mem lodcation s to the new location new and returns the new string.
char *str_dup(const char *s) {

    //Creates a pointer a character pointer for the new word
    char *new;

    //Records the amount of characters in s. The + 1
    //is for the \0
    size_t len = strlen(s) + 1;

    //Allocates enough memory for a new string
    //the duplicate of s.
    new = malloc(len);

    //Takes into account if malloc fails.
    if (new == NULL) {
        free(new);
        return NULL;
    }
    //Copes len characters from mem  location s to new
    memcpy(new, s, len);

    return new;
}

//Creates a node.
Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) calloc(1, sizeof(Node));
    //If n callocs
    if (n) {
        //If a node is null (mainly the head and tail)
        if (oldspeak == NULL && newspeak == NULL) {
            n->oldspeak = NULL;
            n->newspeak = NULL;

        }
        //Stores oldspeak and newspeak.
        else {
            n->oldspeak = str_dup(oldspeak);
            n->newspeak = newspeak == NULL ? NULL : str_dup(newspeak);
        }

        n->next = NULL;
        n->prev = NULL;
    }

    return n;
}

//Deletes a node by freeing the mem locations of oldspeak and newspeak,
//If they are null then dont free however.
void node_delete(Node **n) {
    if ((*n)->oldspeak != NULL) {
        free((*n)->oldspeak);
    }

    if ((*n)->newspeak != NULL) {
        free((*n)->newspeak);
    }
    free(*n);
    *n = NULL;
}

///Helper function for printing nodes
void node_print(Node *n) {
    if (n->newspeak == NULL && n->oldspeak != NULL) {
        printf("%s\n", n->oldspeak);
    }

    if (n->newspeak != NULL && n->oldspeak != NULL) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    }
}
