#include "hash.h"

#include "speck.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Hashtable ADT
struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    bool mtf;
    LinkedList **lists;
};

//Creates an array for pointers to linked lists.
//Also contains the predetermined salts used for hashing words.
HashTable *ht_create(uint32_t size, bool mtf) {

    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (size <= 0) {
        free(ht);
        ht = NULL;
        return ht;
    }
    if (ht) {
        ht->salt[0] = 0x85ae998311115ae3;
        ht->salt[1] = 0xb6fac2ae33a40089;
        ht->size = size;
        ht->mtf = mtf;
        ht->lists = (LinkedList **) calloc(size, sizeof(LinkedList *));
        if (!ht->lists) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

//Frees each ll in the array, then the array then the ll.
void ht_delete(HashTable **ht) {
    for (uint32_t i = 0; i < ht_size(*ht); i++) {
        if ((*ht)->lists[i] != NULL) {
            ll_delete(&((*ht)->lists[i]));
        }
    }
    free((*ht)->lists);
    free(*ht);
    *ht = NULL;
}

//Returns the size of the table.
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

//Uses the hash function to generate an index for a linked list. If the index is
//null then return 0, else return the node for ll_lookup.
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    uint32_t index = (hash(ht->salt, oldspeak)) % ht_size(ht);
    if (ht->lists[index] == NULL) {
        Node *temp;
        temp = NULL;
        return temp;
    }
    return ll_lookup(ht->lists[index], oldspeak);
}

//Inserts a node into a linked list at the generated index.
//If the index is null then create a linked list at that index.
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    uint32_t index = (hash(ht->salt, oldspeak)) % ht_size(ht);
    if (ht->lists[index] == NULL) {
        ht->lists[index] = ll_create(ht->mtf);
    }
    ll_insert(ht->lists[index], oldspeak, newspeak);
}

//Helper function for printing.
void ht_print(HashTable *ht) {
    for (uint32_t i = 0; i < ht_size(ht); i++) {
        if (ht->lists[i] == NULL) {
            printf("NULL\n");
        }

        else {
            ll_print(ht->lists[i]);
            printf("\n");
        }
    }
}
