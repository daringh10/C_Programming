#include "bubble.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//Bubble sort comapres every single value
//starting at A[1] with the value before it,
//up until n - i. It can be extremley inefficient
//for lists in complete reverse, its worst cases failing
//miserably and producing a time complexity of O(n^2).

//Initializes external variables
//for counting moves and compares.
extern uint64_t moves;
extern uint64_t compares;

//This function not only only compares the current
//index value with the previous one but also
//incriments compares.
bool compare_bubble_check(uint32_t *A, uint32_t index) {
    compares += 1;
    if (A[index] < A[index - 1]) {
        return true;
    }

    else {
        return false;
    }
}

//For each iteration through, decrements the total
//iteration length by 1 and will compare
//and swap values when out of order.
void bubble_sort(uint32_t *A, uint32_t n) {

    bool swapped = true;
    uint32_t temp_n = n;
    while (swapped) {

        swapped = false;
        for (uint32_t i = 1; i < temp_n; i += 1) {
            if (compare_bubble_check(A, i)) {
                uint32_t temp = A[i];
                A[i] = A[i - 1];
                A[i - 1] = temp;
                swapped = true;
                moves += 3;
            }
        }
        temp_n -= 1;
    }
}
