#include "bm.h"
#include "hamming.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define OPTIONS ":i:o"

//Courtesy of Proffesor Long. Helper function
//to return lower half of byte.
static uint8_t lower_nibble(uint8_t val) {
    return val & 0x0F;
}

//Courtesy of Proffesor Long. Helper function
//to return upper half of byte.
static uint8_t upper_nibble(uint8_t val) {
    return val >> 4;
}

int main(int argc, char **argv) {

	//Initialize file inputs and set to default if no arguments specified.
    FILE *input_file;
    FILE *output_file;
    struct stat buf;
    input_file = stdin;
    output_file = stdout;

	//Loop through command line to get file arguments
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            input_file = fopen(optarg, "r");

            fchmod(fileno(output_file), buf.st_mode);
            if (input_file == NULL) {
                fprintf(stderr, "Please specify a better input file\n");
                return 0;
            }

            break;

        case 'o':

            output_file = fopen(optarg, "w");
            fstat(fileno(input_file), &buf);

            if (optarg == NULL) {
                fprintf(stderr, "Please specify a better output file\n");
                return 0;
            }

            break;

        default: fprintf(stderr, "Please specify better input\n"); return 0;
        }
    }

	//Initilaize G and H matricies.
    ham_init();
    
    //Create a varialbe "byte" to store the the file input.
    int byte = 0;
    while ((byte = fgetc(input_file)) != EOF) {
    
		//Initialize variables to store upper and lower nibbles of file input.
        uint8_t code_lower;
        uint8_t code_upper;
        uint8_t store_code_lower;
        uint8_t store_code_upper;
		
		//Set the hamming code of the lower and upper nibbles to their respective hamming codes.
        code_lower = lower_nibble(byte);
        code_upper = upper_nibble(byte);
        ham_encode(code_lower, &store_code_lower);
        ham_encode(code_upper, &store_code_upper);

		//Output the hamming codes to file output.
        fputc(store_code_lower, output_file);
        fputc(store_code_upper, output_file);
    }

	//Close files and delete the initialized matricies.
    fclose(input_file);
    fclose(output_file);
    ham_destroy();

    return 0;
}
