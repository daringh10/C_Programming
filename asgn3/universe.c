#include "universe.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//Creating the universe type with 4 tags
//Which help for universe creation and manipulation
struct Universe {
    int rows;
    int cols;
    bool **grid;
    bool toroidal;
};

//Creates a Universe with custom rows/collumns
//And takes in a bool to determine whether its toroidal.
//Allocates enough memory for a Universe and its grid
Universe *uv_create(int rows, int cols, bool toroidal) {

    Universe *u = (Universe *) calloc(1, sizeof(Universe));
    u->rows = rows;
    u->cols = cols;
    u->toroidal = toroidal;
    u->grid = (bool **) calloc(rows, sizeof(bool *)); //2d array to store bools
    for (int r = 0; r < rows; r += 1) {
        u->grid[r] = (bool *) calloc(cols, sizeof(bool));
    }

    return u;
}

//Frees allocated memory.
//Running valgrind reports memory leaks for ncurses
//The only work around is building a debugging version
//Of ncurses
void uv_delete(Universe *u) {
    for (int r = 0; r < (u->rows); r += 1) {
        free(u->grid[r]);
    }
    free(u->grid);
    free(u);
}

//Helper function for accessing a Universe's row number
int uv_rows(Universe *u) {
    return u->rows;
}

//Helper function for accessing a Universe's collumn number
int uv_cols(Universe *u) {
    return u->cols;
}

//Sets a cell in a universe to true
void uv_live_cell(Universe *u, int r, int c) {
    u->grid[r][c] = true;
}

//Sets a cell in a universe to false
void uv_dead_cell(Universe *u, int r, int c) {
    u->grid[r][c] = false;
}

//Returns where a cell is true or false
bool uv_get_cell(Universe *u, int r, int c) {
    return u->grid[r][c];
}

//Opens the specified input file and changes the
//Corresponding cells determined by format "x y" to true
//Returns false if row/col is out of range
bool uv_populate(Universe *u, FILE *infile) {

    int i = 0;
    int x = 0;
    int y = 0;

    while (fscanf(infile, "%d %d\n", &x, &y) != EOF) {

        if (x >= u->rows || y >= u->cols || x < 0 || y < 0) {
            fprintf(stderr, "%d %d is not a valid input\n", x, y);
            return false;
        }

        else {

            uv_live_cell(u, x, y);
        }

        i++;
    }
    fclose(infile);
    return true;
}

//Checks each adjacent cell except for iteself for its state
//By iterating through a mini grid starting at the cell left and up 1
//From its current position. Ignores cells out of bounds unless specified
//By -t argument
int uv_census(Universe *u, int r, int c) {
    int count = 0;
    int temp_c = c - 1;
    int temp_r = r - 1;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            // If the current cell is the starting cell then do nothing for count
            if (temp_c == c && temp_r == r) {
                count += 0;
            }

            // If the cell is non existent in the grid then do nothing for count
            else if (temp_c == u->cols || temp_c == -1 || temp_r == u->rows || temp_r == -1) {
                count += 0;

                // Howeber if universe is toroidal then ignore bounds and
                // Treat each collumn and row as circular, so after the max value
                // The next row/col would be row/col 0
                if (u->toroidal == true) {
                    int new_r = 0;
                    int new_c = 0;

                    if (temp_r == -1) {
                        new_r = u->rows - 1;
                    } else if (temp_r == u->rows) {
                        new_r = 0;
                    }

                    else {
                        new_r = temp_r;
                    }

                    if (temp_c == -1) {
                        new_c = u->rows - 1;
                    } else if (temp_c == u->cols) {
                        new_c = 0;
                    }

                    else {
                        new_c = temp_c;
                    }

                    if (u->grid[new_r][new_c] == true) {
                        count += 1;
                    }
                }
            }

            //If the the cell state is true then incriment count by 1
            else if (u->grid[temp_r][temp_c] == true) {
                count += 1;
            }

            //If none of the cases apply, then the cell is dead so incriment by 0
            else {

                count += 0;
            }

            temp_c += 1;
        }
        temp_c = temp_c - 3;
        temp_r += 1;
    }

    return count;
}
//Prints the universe to specified outfile
//Prints 'o' for state=true and '.' for state=false
void uv_print(Universe *u, FILE *outfile) {
    for (int i = 0; i < (u->rows); i++) {
        for (int j = 0; j < (u->cols); j++) {
            if (u->grid[i][j] == false) {
                fprintf(outfile, ".");
            }

            else if (u->grid[i][j] == true) {
                fprintf(outfile, "o");
            }
        }
        fprintf(outfile, "\n");
    }
}
