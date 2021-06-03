#include "bm.h"
#include "hamming.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define OPTIONS ":io:"

//Courtesy of Proffesor Long, function that returns a byte of 2 nibbles
//that is the conjuction of 2 nibbles forming 1 byte. 
static uint8_t pack_byte(uint8_t upper, uint8_t lower) {
    return (upper << 4) | (lower & 0x0F);
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
            fstat(fileno(input_file), &buf);

            if (input_file == NULL) {
                fprintf(stderr, "Please specify a better input file\n");
                return 0;
            }

            break;

        case 'o':

            output_file = fopen(optarg, "w");
            fchmod(fileno(output_file), buf.st_mode);

            if (optarg == NULL) {
                fprintf(stderr, "Please specify a better output file\n");
                return 0;
            }

            break;

        default: fprintf(stderr, "Please specify better input\n"); return 0;
        }
    }


	//Create variables to store data regarding erros.
    int bytes_processed = 0;
    int bytes_not_corrected = 0;
    int bytes_corrected = 0;
    double error_rate = 0;
    int HAM_ERR_READER = 0;

	//Initialize G and H matricies as well as create variables to store the hamming
	//codes of the upper and lower nibbles of a byte.
    ham_init();
    int lower_byte = 0;
    int upper_byte = 0;
    
    while ((lower_byte = fgetc(input_file)) != EOF) {

        upper_byte = fgetc(input_file);

		//Create variables to store nibbles regarding the lower
		//and upper bytes.
        uint8_t store_code_lower;
        uint8_t store_code_upper;

        HAM_ERR_READER = ham_decode(lower_byte, &store_code_lower);


		//Document data regarding amount of errors, errors corrected, and
		//erros uncorrected.
        if (HAM_ERR_READER == HAM_ERR_OK) {
            bytes_corrected += 1;
        }

        else if (HAM_ERR_READER == HAM_ERR) {
            bytes_not_corrected += 1;
        }

        else {
        }

        HAM_ERR_READER = ham_decode(upper_byte, &store_code_upper);
        if (HAM_ERR_READER == HAM_ERR_OK) {
            bytes_corrected += 1;
        }

        else if (HAM_ERR_READER == HAM_ERR) {
            bytes_not_corrected += 1;
        }

        else {
        }

		//Form the original input byte and output it.
        uint8_t original_byte = pack_byte(store_code_upper, store_code_lower);
        fputc(original_byte, output_file);

        bytes_processed += 2;
    }
    
    //Destroy the G and H matricies and close opened files.
    ham_destroy();
    fclose(input_file);
    fclose(output_file);

	//Print error data.
    error_rate = (double) bytes_not_corrected / (double) bytes_processed;

    fprintf(stderr, "Total bytes processed: %d\n", bytes_processed);
    fprintf(stderr, "Uncorrected errors: %d\n", bytes_not_corrected);
    fprintf(stderr, "Corrected errors: %d\n", bytes_corrected);
    fprintf(stderr, "Error rate: %7.6f\n", (error_rate));

    return 0;
}
