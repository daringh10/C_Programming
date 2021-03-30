#include "io.h"

#include "code.h"
#include "endian.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BITS_PER_UNIT 8

//Initializing vars for sym buffer
static uint8_t symbols[BLOCK];
static int index = 0;
static int end;

//Vars needed for statisitc analyzing
uint64_t total_syms = 0.0;
uint64_t total_bits = 0.0;

//Initializing vars for bit buffer
static uint8_t bit_buffer[BLOCK];
static int bit_index = 0;
static uint32_t bit_end;



//Helper function for finding bytes from bits
uint32_t bytes(uint32_t bits) {
    if (bits == 0) {
        return 0;
    } else {
        return bits / 8;
    }
}

//All these are helper functions for setting/getting/clearing bits from
//a buffer or a uint8_t or unint16_t. Uses bit manipulation
void set_bit() {
    bit_buffer[bytes(bit_index)] |= (uint8_t) 0x01 << (bit_index % BITS_PER_UNIT);
}

void set_bit_code(uint16_t *code, int i) {
    *code |= (uint16_t) 0x01 << i;
}

void set_bit_sym(uint8_t *sym, int i) {
    *sym |= (uint8_t) 0x01 << i;
}

uint8_t get_bit_code(uint16_t code, uint32_t i) {
    return ((code) & (0x01 << i)) >> (i);
}

uint8_t get_bit_buf() {
    return ((bit_buffer[bytes(bit_index)]) & (0x01 << bit_index % BITS_PER_UNIT))
           >> (bit_index % BITS_PER_UNIT);
}

uint8_t get_bit_sym(uint8_t sym, uint32_t i) {
    return ((sym) & (0x01 << i)) >> (i);
}

void clr_bit() {
    bit_buffer[bytes(bit_index)] &= ~((uint8_t) 0x01 << (bit_index % BITS_PER_UNIT));
}

void clr_bit_code(uint16_t *code, int i) {
    *code &= ~((uint16_t) 0x01 << i);
}

void clr_bit_sym(uint8_t *sym, int i) {
    *sym &= ~((uint8_t) 0x01 << i);
}



//Function to read up to to_read bytes from an infile into the buf.
int read_bytes(int infile, uint8_t *buf, int to_read) {
    int total = 0;
    int bytes_read = 0;

    do {

        bytes_read = read(infile, buf, to_read);
        total += bytes_read;
        to_read -= bytes_read;
        buf += bytes_read;

        if (bytes_read <= 0) {
            break;
        }

    } while (to_read > 0);

    return total;
}


//Function to write up to to_write bytes from a buffer into an outfile.
int write_bytes(int outfile, uint8_t *buf, int to_write) {
    int total = 0;
    int bytes_written = 0;
    total_bits += to_write;

    do {

        bytes_written = write(outfile, buf, to_write);
        total += bytes_written;
        to_write -= bytes_written;
        buf += bytes_written;

        if (bytes_written <= 0) {
            break;
        }

    } while (to_write > 0);

    return total;
}

//Reads in header bytes from an infile into the header buf.
void read_header(int infile, FileHeader *header) {
    read_bytes(infile, (uint8_t *) header, sizeof(FileHeader));
    if (big_endian()) {
        header->magic = swap32(header->magic);
        header->protection = swap16(header->protection);
    }
}

//Writes out bytes from a header buf to an outfile.
void write_header(int outfile, FileHeader *header) {
    if (big_endian()) {
        header->magic = swap32(header->magic);
        header->protection = swap16(header->protection);
    }
    write_bytes(outfile, (uint8_t *) header, sizeof(FileHeader));
}

//Reads bytes from the infile and adds them 1 byte 
//at a time to the symbol index, each spot is 1 character
bool read_sym(int infile, uint8_t *sym) {
    if (index == end && index != 0) {

        return false;
    }
    if (index == 0) {
        end = read_bytes(infile, symbols, BLOCK);
        total_syms += end;
    }
    *sym = symbols[index];
    index += 1;

    if (index == BLOCK) {
        index = 0;
    }
    return true;
}

//Writes pairs from the bit_buffer to outfile, AKA the compressed file.
void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen) {
    if (big_endian()) {
        code = swap16(code);
    }

    for (int i = 0; i < bitlen; i++) {
        if (get_bit_code(code, i) == 1) {
            set_bit();

        }

        else {
            clr_bit();
        }

        bit_index += 1;

        if (bytes(bit_index) == BLOCK) {

            write_bytes(outfile, bit_buffer, BLOCK);
            bit_index = 0;
        }
    }

    for (int i = 0; i < 8; i++) {
        if (get_bit_sym(sym, i) == 1) {
            set_bit();
        }

        else {
            clr_bit();
        }

        bit_index += 1;
        if (bytes(bit_index) == BLOCK) {

            write_bytes(outfile, bit_buffer, BLOCK);

            bit_index = 0;
        }
    }
}

//Writes remaining bytes from bit buffer to outfile
void flush_pairs(int outfile) {

    write_bytes(outfile, bit_buffer, bytes(bit_index));
    bit_index = 0;
}

//Read in pairs from the compressed file and sets the appropriate spot in
//the bit index.
bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen) {

    uint16_t temp_code = 0;
    for (int i = 0; i < bitlen; i++) {
        if ((bytes(bit_index) == bit_end) && (bytes(bit_index) != 0)) {

            return false;
        }

        if (bit_index == 0) {

            bit_end = read_bytes(infile, bit_buffer, BLOCK);
            total_syms += bit_end;
        }

        if (get_bit_buf() == 1) {
            set_bit_code(&temp_code, i);
        }

        else {
            clr_bit_code(&temp_code, i);
        }

        bit_index += 1;

        if (bytes(bit_index) == BLOCK) {
            bit_index = 0;
        }
    }
    *code = temp_code;
    if (big_endian()) {
        *code = swap16(*code);
    }

    uint8_t temp_sym = 0;
    for (int i = 0; i < 8; i++) {
        if ((bytes(bit_index) == bit_end) && (bytes(bit_index) != 0)) {

            return false;
        }
        if (bit_index == 0) {
            bit_end = read_bytes(infile, bit_buffer, BLOCK);
            total_syms += bit_end;
        }

        if (get_bit_buf() == 1) {
            set_bit_sym(&temp_sym, i);
        }

        else {
            clr_bit_sym(&temp_sym, i);
        }

        bit_index += 1;
        if (bytes(bit_index) == BLOCK) {
            bit_index = 0;
        }

        //temp
        if ((bytes(bit_index) == bit_end) && (bytes(bit_index) != 0)) {

            return false;
        }
    }
    *sym = temp_sym;

    return true;
}

//Writes out words to the outfile. Access's the bit buffer and 
//finds the matching bit_index from the word's symbol index.
void write_word(int outfile, Word *w) {

    for (uint32_t i = 0; i < w->len; i++) {

        symbols[index] = w->syms[i];
        index += 1;

        if (index == BLOCK) {
            write_bytes(outfile, symbols, BLOCK);
            index = 0;
        }
    }
}
//Writes out all remaining words to outfile.
void flush_words(int outfile) {

    write_bytes(outfile, symbols, index);
}
