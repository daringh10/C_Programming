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

//Compression statistics initialization
uint64_t total_syms;
uint64_t total_bits;

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

int main(int argc, char **argv) {

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
                fprintf(stderr, "Failed to open infile %s.\n", optarg);
                return 0;
            }
            break;

        case 'o':

            outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0600);
            if (outfile == -1) {
                fprintf(stderr, "Failed to open outfile %s.\n", optarg);
                return 0;
            }
            break;
        default: print_error(argv[0]); return 0;
        }
    }

    //Allocates memory for the file header, which will be written onto
    //the compressed file once it's been initialized with a magic number
    //and a protection sequence.
    FileHeader *header = (FileHeader *) calloc(1, (sizeof(FileHeader)));
    struct stat statbuf;
    fstat(infile, &statbuf);
    header->magic = MAGIC;
    header->protection = statbuf.st_mode;
    fchmod(outfile, header->protection);
    write_header(outfile, header);

    //Lempel-Ziv Algorithm Implementation
    TrieNode *root = trie_create();
    TrieNode *curr_node = root;
    TrieNode *prev_node = NULL;
    uint8_t curr_sym = 0;
    uint8_t prev_sym = 0;
    uint16_t next_code = START_CODE;

    while (read_sym(infile, &curr_sym)) {
        TrieNode *next_node = trie_step(curr_node, curr_sym);
        if (next_node != NULL) {
            prev_node = curr_node;
            curr_node = next_node;
        }

        else {
            write_pair(outfile, curr_node->code, curr_sym, min_bit_len(next_code));
            curr_node->children[curr_sym] = trie_node_create(next_code);
            curr_node = root;
            next_code = next_code + 1;
        }

        if (next_code == MAX_CODE) {
            trie_reset(root);
            curr_node = root;
            next_code = START_CODE;
        }
        prev_sym = curr_sym;
    }

    if (curr_node != root) {
        write_pair(outfile, prev_node->code, prev_sym, min_bit_len(next_code));
        next_code = (next_code + 1) % MAX_CODE;
    }
    write_pair(outfile, STOP_CODE, 0, min_bit_len(next_code));
    flush_pairs(outfile);

    //Frees allocated memory
    trie_delete(root);
    free(header);
    header = NULL;

    //Code to print compression statisitcs.
    if (print_statistics) {
        double space_saving = 0.0;
        double compressed_size = (total_bits);
        double uncompressed_size = (total_syms);
        space_saving = (double) 100 * (1 - (compressed_size / uncompressed_size));
        printf("Compressed file size: %lu\n", (total_bits));
        printf("Uncompressed file size: %lu\n", total_syms);
        printf("Space saving: %4.2lf%%\n", space_saving);
    }
    //Close all neccesary files.
    close(infile);
    close(outfile);
    return 0;
}
