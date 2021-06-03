#include "bv.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BITS_PER_UNIT 8

//Helper function that makes acquiring bits from bytes easier
//The array looks like [0,0] if length is 16 because that first value
//Holds 8 bits, and this function simply allocates the correct amount.
//Saves some tediom
uint32_t bytes(uint32_t bits) {
    if (bits == 0) {
        return 1;
    } else {
        return (bits % BITS_PER_UNIT == 0) ? bits / BITS_PER_UNIT : bits / BITS_PER_UNIT + 1;
    }
}

//BitVector ADT
struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

//Creates an array of uint8_t's that act as the indexs for words, more specifically,
//each word will set 3 bits to 1 when inserted (thats in bloom filter code).
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) (calloc(1, sizeof(BitVector)));

    if (bv) {
        bv->length = length;
        bv->vector = (uint8_t *) (calloc(bytes(bv->length), sizeof(uint8_t)));
        if (!bv->vector) {
            free(bv);
            bv = NULL;
        }
    }
    return bv;
}

//Frees the array and sets the bv pointer to null.
void bv_delete(BitVector **bv) {
    free((*bv)->vector);
    free(*bv);
    *bv = NULL;
}

//Returns the length of a bv.
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

//Sets a bit to 1 in the array.
void bv_set_bit(BitVector *bv, uint32_t i) {
    bv->vector[i / BITS_PER_UNIT] |= (uint8_t) 0x01 << (i % BITS_PER_UNIT);
}

//Sets a bit to 0 in the array.
void bv_clr_bit(BitVector *bv, uint32_t i) {
    bv->vector[i / BITS_PER_UNIT] &= ~((uint8_t) 0x01 << (i % BITS_PER_UNIT));
}

//Returns the value of a bit in the array. Helpful for looking up words in the bf.
uint8_t bv_get_bit(BitVector *bv, uint32_t i) {
    return ((bv->vector[i / BITS_PER_UNIT]) & (0x01 << i % BITS_PER_UNIT)) >> (i % BITS_PER_UNIT);
}

//Helper function for printing a bv.
void bv_print(BitVector *bv) {
    for (uint32_t i = 0; i < bv_length(bv); i++) {
        if (bv_get_bit(bv, i) == 1) {
            printf("1");
        }

        else {
            printf("0");
        }
    }
}
