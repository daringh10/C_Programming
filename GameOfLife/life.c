#include "universe.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "stn:i:o"
#define DELAY   50000
#define MAX_GEN 4294967294

//Bools to determine whether the universe is toroidal
//And whether its silenced
bool tflag, silenced = false;

int main(int argc, char **argv) {

    //Default amount of generations is 100
    int generations = 100;

    //Initializes input files
    FILE *input_file;
    FILE *output_file;
    output_file = stdout;

    //If nothing after the execute is inputted then show user how to use program
    if (argc < 2) {
        fprintf(stderr, "Program usage: %s -[stn:i:o]\n", argv[0]);
        printf("  -s             silences ncurses\n");
        printf("  -t             game will be played on toroidal universe\n");
        printf("  -n [int]       specifies number of generations (default 100)\n");
        printf("  -i [filepath]  specifies input file\n");
        printf("  -o [filepath]  specifies outputfile (default stdout)\n");
        return 0;
    }

    //Iterate through command line to determine how to create the universe
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {

            //Must specify a filepath for i or program will end
        case 'i':
            input_file = fopen(optarg, "r");
            if (input_file == NULL) {
                fprintf(stderr, "Please specify a better input file\n");
                return 0;
            }
            break;

        //If o is specified then must determine the filepath or else program ends
        //If no o is typed then prints to stdout
        case 'o':
            output_file = fopen(optarg, "w");

            if (optarg == NULL) {
                fprintf(stderr, "Please specify a better output file\n");
                fprintf(stderr, "Default will be stdout if none specified\n");
                return 0;
            }
            break;

        //These cases determine gen size, toroidail y/n, and whether to run curses
        //Also handles if generation size is faulty
        case 't': tflag = true; break;
        case 's': silenced = true; break;
        case 'n':
            generations = atoi(optarg);

            if (generations <= 0 || (generations > 5)) {
                fprintf(stderr, "Please specify a better generation number\n");
                return false;
            }

            break;

        default: fprintf(stderr, "Please specify better input\n"); return 0;
        }
    }

    //Initializes the row and cols for the universe
    //i stans for input
    int i_rows = 0;
    int i_cols = 0;

    //Scans the input file to determine the row and col length
    fscanf(input_file, "%d %d\n", &i_rows, &i_cols);

    //Initializes and creates the 2 universes
    Universe *A;
    Universe *B;
    A = uv_create(i_rows, i_cols, tflag);
    B = uv_create(i_rows, i_cols, tflag);

    //Populates the universe unless a point is out of range
    if (uv_populate(A, input_file) == false) {
        fprintf(stderr, "Input out of range\n");
        return 0;
    }

    //Determines whether to use ncurses or not
    //If so, then at the beginning of each generation
    //Ncurses will print
    int count = 0;
    if (silenced == false) {
        initscr();
        curs_set(FALSE);
        for (int gen = 0; gen < generations; gen++) {
            clear();
            for (int r = 0; r < uv_rows(A); r += 1) {
                for (int c = 0; c < uv_cols(A); c += 1) {
                    if (uv_get_cell(A, r, c) == true) {
                        mvprintw(r, c, "o");
                    }

                    else {
                        mvprintw(r, c, "");
                    }
                }
            }

            refresh();
            usleep(DELAY);

            //For each cell in the grid check the following:
            for (int r = 0; r < uv_rows(A); r += 1) {
                for (int c = 0; c < uv_cols(A); c += 1) {
                    count = uv_census(A, r, c);

                    //If a cell is alive and its census is 2 or 3 then it survives
                    //Otherwise the misery of death awaits it :(
                    if (uv_get_cell(A, r, c) == true) {
                        if (count == 3 || count == 2) {
                            uv_live_cell(B, r, c);

                        }

                        else {
                            uv_dead_cell(B, r, c);
                        }

                    }

                    //IF a cell is dead but its census is 3, then it survives
                    //Like a good little cell and flourishes... for now
                    //And otherwise is dies... :(
                    else if (uv_get_cell(A, r, c) == false) {

                        if (count == 3) {

                            uv_live_cell(B, r, c);

                        }

                        else {
                            uv_dead_cell(B, r, c);
                        }

                    }

                    //If a cell meets none of these conditions then it dies
                    else {
                        uv_dead_cell(B, r, c);
                    }
                }
            }

            //Create a pointer to a universe called temp that will house
            //Universe A data while we update universe A with universe B data
            //Which we transfered to during our generation
            Universe *temp = A;
            A = B;
            B = temp;
        }

        endwin();
    }

    //If ncurses is silenced, then runs through standard
    //Generation protocol
    else {
        for (int gen = 0; gen < generations; gen++) {
            for (int r = 0; r < uv_rows(A); r += 1) {
                for (int c = 0; c < uv_cols(A); c += 1) {
                    count = uv_census(A, r, c);

                    if (uv_get_cell(A, r, c) == true) {
                        if (count == 3 || count == 2) {
                            uv_live_cell(B, r, c);

                        }

                        else {
                            uv_dead_cell(B, r, c);
                        }

                    } else if (uv_get_cell(A, r, c) == false) {

                        if (count == 3) {

                            uv_live_cell(B, r, c);

                        }

                        else {
                            uv_dead_cell(B, r, c);
                        }

                    }

                    else {
                        uv_dead_cell(B, r, c);
                    }
                }
            }
            Universe *temp = A;
            A = B;
            B = temp;
        }
    }

    //Print the final generation of A to the output file
    uv_print(A, output_file);

    //Frees all allocated memory from calloc
    uv_delete(A);
    uv_delete(B);

    return 0;
}
