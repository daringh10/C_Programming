#include "bf.h"

#include "speck.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//BloomFilter ADT
struct BloomFilter {
    uint64_t priamry[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};

//The filter is a bv and the salts are for hashing words.
BloomFilter *bf_create(uint32_t size) {

    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));

    if (size == 0) {
        free(bf);
        bf = NULL;
        return bf;
    }

    if (bf) {

        bf->priamry[0] = 0x02d232593fbe42ff;
        bf->priamry[1] = 0x3775cfbf0794f152;

        bf->secondary[0] = 0xc1706bc17ececc04;
        bf->secondary[1] = 0xe9820aa4d2b8261a;

        bf->tertiary[0] = 0xd37b01df0ae8f8d0;
        bf->tertiary[1] = 0x911d454886ca7cf7;

        bf->filter = bv_create(size);
        if (!bf->filter) {
            free(bf);
            bf = NULL;
        }
    }
    return bf;
}

//Deletes a bloom filter adt by first freeing the bv (filter) and setting the
//bf pointer to null after freeing it.
void bf_delete(BloomFilter **bf) {
    bv_delete(&((*bf)->filter));
    free(*bf);
    *bf = NULL;
}

//Returns the length of the filter.
uint32_t bf_length(BloomFilter *bf) {
    return bv_length(bf->filter);
}

//Inserts a word into the bloom filter. A word is hashed with 3 salts, so 3
//Indices will be set to 1. That combination will guarantee that the word is enterd.
//Remember that if another word sets the indices and another word shares the same set of indices
//after being hashed, that doesnt guarantee that the word has been hashed yet.
void bf_insert(BloomFilter *bf, char *oldspeak) {
    bv_set_bit(bf->filter, hash(bf->priamry, oldspeak) % bf_length(bf));
    bv_set_bit(bf->filter, hash(bf->secondary, oldspeak) % bf_length(bf));
    bv_set_bit(bf->filter, hash(bf->tertiary, oldspeak) % bf_length(bf));
}

//Hashes a word 3 times with the salts and returs true if all are set.
//Doesn't guarantee the word is in the filter however.
bool bf_probe(BloomFilter *bf, char *oldspeak) {
    if (bv_get_bit(bf->filter, hash(bf->priamry, oldspeak) % bf_length(bf)) == 1
        && bv_get_bit(bf->filter, hash(bf->secondary, oldspeak) % bf_length(bf)) == 1
        && bv_get_bit(bf->filter, hash(bf->tertiary, oldspeak) % bf_length(bf)) == 1) {
        return true;
    }

    else {
        return false;
    }
}

//Helper function for printing a bf.
void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
}
