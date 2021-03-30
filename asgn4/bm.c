#include "bm.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BITS_PER_UNIT 8

//Helper function that makes acquiring bits from bytes easier
//Saves some tediom
uint32_t bytes(uint32_t bits) {
    if (bits == 0) {
        return 1;
    } else {
        return (bits % BITS_PER_UNIT == 0) ? bits / BITS_PER_UNIT : bits / BITS_PER_UNIT + 1;
    }
}

//Initializes BitMatrix ADT
struct BitMatrix {
    int rows;
    int cols;
    uint8_t **mat;
};

//Allocates memory for the Bit Matrix, holding the number of bytes
//Amount of collumns. Of course each byte has 8 bits which will represent the
//Positions of the matrix values.
BitMatrix *bm_create(uint32_t rows, uint32_t cols) {
    BitMatrix *m = (BitMatrix *) calloc(1, sizeof(BitMatrix));
    if (m == NULL) {
        return NULL;
    }

    m->rows = rows;
    m->cols = cols;

    m->mat = (uint8_t **) calloc(rows, sizeof(uint8_t *));
    if (m->mat == NULL) {
        return NULL;
    }

    for (uint32_t i = 0; i < rows; i++) {
        m->mat[i] = (uint8_t *) calloc(bytes(cols), sizeof(uint8_t));
        if (m->mat[i] == NULL) {
            return NULL;
        }
    }

    return m;
}

//Clears memory from the inside out. Starts from the rows and works its
//way up to the memory allocated for the BitMatrix.
void bm_delete(BitMatrix **m) {

    for (uint32_t r = 0; r < (bm_rows(*m)); r += 1) {
        free((*m)->mat[r]);
    }

    free((*m)->mat);

    free(*m);
    *m = NULL;
}

//Returns the amount of rows in the Bit Matrix.
uint32_t bm_rows(BitMatrix *m) {
    return m->rows;
}

//Returns the amount of collumns in the Bit Matrix.
uint32_t bm_cols(BitMatrix *m) {
    return m->cols;
}

//Sets a bit at a given [row][col] in the Bit Matrix.
void bm_set_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    m->mat[r][c / BITS_PER_UNIT] |= ((uint8_t) 0x1 << (c % BITS_PER_UNIT));
    return;
}

//Sets a bit to 0 at a position in the Bit Matrix.
void bm_clr_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    uint8_t byte = m->mat[r][c / BITS_PER_UNIT];
    uint8_t temp = ~(0x1 << c % BITS_PER_UNIT);
    byte = byte & temp;
    m->mat[r][c / BITS_PER_UNIT] = byte;
    return;
}

//Returns the bit at a position in the Bit Matrix.
uint8_t bm_get_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    uint8_t byte = m->mat[r][c / BITS_PER_UNIT];
    byte = byte & (0x1 << c % BITS_PER_UNIT);
    return byte >> c % BITS_PER_UNIT;
}

//Easy way to display the Bit Matrix.
void bm_print(BitMatrix *m) {
    for (uint8_t i = 0; i < m->rows; i++) {
        for (uint8_t j = 0; j < m->cols; j++) {
            if (bm_get_bit(m, i, j) == 1) {
                printf("1");
            }

            else {
                printf("0");
            }
        }
        printf("\n");
    }
}
