#include "bubble.h"
#include "heap.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "utilities.h"

#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS      "absqhr:n:p:"
#define SEEDMAX      4294967295
#define MAX_COLLUMNS 5

//External variable for keeping track of
//compares and swaps.
extern uint64_t moves;
extern uint64_t compares;

//Sets the default seed, array length, and number
//of elements to print.
uint32_t seed = 7092016;
uint32_t array_size = 100;
uint32_t num_of_elements = 100;

//Function to fill the main array
void fill_array(uint32_t *A, uint32_t n) {
    srand(seed);
    //30 consecutive bits enabled.
    uint32_t random_mask = 0x03FFFFFFF;

    //Fills array with random elements.
    for (uint32_t i = 0; i < n; i++) {
        A[i] = (rand() & random_mask);
    }
}

//Function to format and print the array correctly.
void print_array(uint32_t *A, uint32_t p, uint32_t n) {

    printf("%d elements, %ld moves, %ld compares\n", n, moves, compares);

    if (n < p) {
        for (uint32_t i = 0; i < n; i++) {
            if ((i > 0) && (i % 5 == 0)) {
                printf("\n");
            }
            printf("%13" PRIu32, A[i]);
        }
        printf("\n");

    }

    else {
        for (uint32_t i = 0; i < p; i++) {
            if ((i > 0) && (i % MAX_COLLUMNS == 0)) {
                printf("\n");
            }
            printf("%13" PRIu32, A[i]);
        }
        printf("\n");
    }
}

//This will be called accordingly, for bad inputs and such.
void print_error() {
    fprintf(stderr, "SYNOPSIS\n"
                    "   A collection of comparison-based sorting algorithms.\n\n"

                    "USAGE\n"
                    "   ./sorting [-absqh] [-n length] [-p elements] [-r seed]\n\n"

                    "OPTIONS\n"
                    "   -a              Enable all sorts.\n"
                    "   -b              Enable Bubble Sort.\n"
                    "   -s              Enable Shell Sort.\n"
                    "   -q              Enable Quick Sort.\n"
                    "   -h              Enable Heap Sort.\n"
                    "   -n length       Specify number of array elements.\n"
                    "   -p elements     Specify number of elements to print.\n"
                    "   -r seed         Specify random seed.\n");
}

int main(int argc, char **argv) {

    //When 1 or less argument values are inputed an error has occcured.
    if (argc < 2) {
        fprintf(stderr, "Select at least one sort to perform\n");
        print_error();
        return 0;
    }

    //Set the list that will hold all the sorts to empty, or 0 since its a number.
    Set sort_list = set_empty();

    //Easier to identify sorts especially with labels attached.
    typedef enum sorts { all = 0, bubble = 1, shell = 2, quick = 3, heap = 4 } sorts;

    //Simply adds sorts to the set, while also modifying print and array options.
    //If a is inputted then add all sorts to set.
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            for (sorts s = bubble; s <= heap; s++) {
                sort_list = set_insert(sort_list, s);
            }
            break;

        case 'b': sort_list = set_insert(sort_list, 1); break;

        case 's': sort_list = set_insert(sort_list, 2); break;

        case 'q': sort_list = set_insert(sort_list, 3); break;

        case 'h': sort_list = set_insert(sort_list, 4); break;

        case 'r':
            seed = atoi(optarg);
            seed = (uint32_t)(seed); //converting to unsigned int
            break;

        case 'n': array_size = atoi(optarg); break;
        case 'p': num_of_elements = atoi(optarg); break;

        default: print_error(); return 0;
        }
    }

    //If the set is empty then nothing to do, end program.
    if (sort_list == set_empty()) {
        print_error();
        return 0;
    }

    //Create the main array, and allocate enough memory for array_size elements
    //Of unsigned 32 bits size each.
    uint32_t *main_arr = (uint32_t *) calloc(array_size, sizeof(uint32_t));

    //For each sort in the set, fill the array, sort the list with the
    //right sort, display details, and set moves and compares to 0;
    for (sorts s = bubble; s <= heap; s++) {
        if (set_member(sort_list, s)) {
            switch (s) {

            case all: break;

            case bubble:

                fill_array(main_arr, array_size);
                bubble_sort(main_arr, array_size);
                printf("Bubble Sort\n");
                print_array(main_arr, num_of_elements, array_size);

                moves = 0;
                compares = 0;
                break;

            case shell:

                fill_array(main_arr, array_size);
                shell_sort(main_arr, array_size);
                printf("Shell Sort\n");
                print_array(main_arr, num_of_elements, array_size);

                moves = 0;
                compares = 0;
                break;

            case quick:

                fill_array(main_arr, array_size);
                quick_sort(main_arr, array_size);
                printf("Quick Sort\n");
                print_array(main_arr, num_of_elements, array_size);

                moves = 0;
                compares = 0;
                break;

            case heap:

                fill_array(main_arr, array_size);
                heap_sort(main_arr, array_size);
                printf("Heap Sort\n");
                print_array(main_arr, num_of_elements, array_size);

                moves = 0;
                compares = 0;
                break;
            }
        }
    }

    //Frees the space in memory allocated to the array.
    free(main_arr);
    main_arr = NULL;
    return 0;
}
