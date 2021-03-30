#include "shell.h"

#include "gaps.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

extern uint64_t moves;
extern uint64_t compares;

//Function to compare whether the index is greater than or equal to the
//current gap length. Increments compares by 1.
bool compare_check(uint32_t index, uint32_t *gaps, uint32_t gap_counter) {
    if (index >= gaps[gap_counter]) {
        compares += 1;
        return true;
    }

    else {
        return false;
    }
}

void shell_sort(uint32_t *A, uint32_t n) {

    //This shell sort iterates through a list of predetermined gap
    //sizes, called the "3-smooth". For each of those gaps,
    //It first sets a variable j and i to the index of the array[gap_size]
    //element and compares that value with the value gap_size to the left
    //of it.
    for (uint32_t gap_counter = 0; gap_counter < GAPS; gap_counter++) {
        for (uint32_t i = gaps[gap_counter]; i < n; i++) {
            uint32_t j = i;
            uint32_t temp = A[i];

            while (compare_check(j, gaps, gap_counter) && temp < A[j - gaps[gap_counter]]) {
                uint32_t temp1 = A[j];
                A[j] = A[j - gaps[gap_counter]];
                A[j - gaps[gap_counter]] = temp1;

                j -= gaps[gap_counter];
                moves += 3;
            }
            A[j] = temp;
        }
    }
}
