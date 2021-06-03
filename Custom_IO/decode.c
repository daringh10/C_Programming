#include "code.h"
#include "io.h"
#include "trie.h"
#include "word.h"

#include <fcntl.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define OPTIONS "vi:o:"

//Variables to keep track of statistic information
uint64_t total_syms;
uint64_t total_bits;

//Function to print errors
void print_error(char *argv) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "	Decompresses files with the LZ78 decompression algorithm.\n"
        "	Used with files compressed with the corresponding encoder.\n\n"
        "USAGE\n"
        "%s [-vh] [-i input] [-o output]\n\n"
        "OPTIONS\n"
        "	-v         Display decompression statistics\n"
        "	-i input   Specify input to decompress (default: stdin)\n"
        "	-o output  Specify output of decompressed input (default: stdout)\n"
        "	-h         Display program usage\n",
        argv);
}

//Function to calculate the minimum number of bits neccesary to represent a 
//uint16
int min_bit_len(uint16_t code) {

    int max_length = 15;
    uint16_t bit_mask = 0x8000;
    for (uint16_t i = 0; i < 16; i++) {
        if (((bit_mask & code) >> (max_length - i)) == 1) {
            return (max_length - i) + 1;
        }
        bit_mask = bit_mask >> 1;
    }

    return 0;
}

int main(int argc, char **argv) {
	
	//Need to initialize external variables
    total_syms = 0.0;
    total_bits = 0.0;

	//More Initializing things
    bool print_statistics = false;
    int infile = STDIN_FILENO;
    int outfile = STDOUT_FILENO;
    int opt = 0;

	//Checks program arguments for correct input
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'v': print_statistics = true; break;
        case 'i':
            infile = open(optarg, O_RDONLY);

            if (infile == -1) {
                fprintf(stderr, "Failed to open %s.\n", optarg);
                return 0;
            }
            break;

        case 'o':
            outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0600);
            if (outfile == -1) {
                fprintf(stderr, "Failed to open %s.\n", optarg);
                return 0;
            }
            break;

        default: print_error(argv[0]); return 0;
        }
    }

    //Creates a file header struct and allocates memory onto the heap.
    //Reads in header bytes from the compressed file, and modifies the outfile
    //with the correct permissions, also checks header->magic.
    FileHeader *header = (FileHeader *) calloc(1, (sizeof(FileHeader)));
    read_header(infile, header);
    fchmod(outfile, header->protection);
    if (header->magic != 0xBAADBAAC) {
        fprintf(stderr, "Incorrect magic number");
        return 1;
    }

	//Lempel-Ziv Decoding Algorithm.
    WordTable *wt = wt_create();
    uint16_t curr_code = 0;
    uint8_t curr_sym = 0;
    uint16_t next_code = START_CODE;
    while (read_pair(infile, &curr_code, &curr_sym, min_bit_len(next_code))) {

        wt[next_code] = word_append_sym(wt[curr_code], curr_sym);
        write_word(outfile, wt[next_code]);

        next_code = next_code + 1;
        if (next_code == MAX_CODE) {
            wt_reset(wt);

            next_code = START_CODE;
        }
    }
    flush_words(outfile);

	//Frees all allocated memory
    wt_delete(wt);
    free(header);
    header = NULL;

	//Code to print compression statisitcs.
    if (print_statistics) {
    	double space_saving = 0.0;
   		double compressed_size = (total_syms);
   	 	double uncompressed_size = (total_bits);
    	space_saving = (double) 100 * (1 - (compressed_size / uncompressed_size));
        printf("Compressed file size: %lu\n", (total_syms));
        printf("Uncompressed file size: %lu\n", total_bits);
        printf("Space saving: %4.2lf%%\n", space_saving);
    }

	//Close all neccesary files.
    close(outfile);
    close(infile);

    return 0;
}
