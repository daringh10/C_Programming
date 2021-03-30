
#include "quick.h"

#include "stack.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//Initializes external variables
//for counting moves and compares.
extern uint64_t moves;
extern uint64_t compares;

//Function to compare the value at the current index to the pivot
//and return true or false. Also increments compares.
bool compare_quick1(uint32_t *A, uint32_t index, uint32_t pivot) {
    compares += 1;
    if (A[index] < pivot) {
        return true;
    }

    else {
        return false;
    }
}

//Function for the other instance of comparing. If the index is greater than pivot
//return true else false. Increments compares by 1.
bool compare_quick2(uint32_t *A, uint32_t index, uint32_t pivot) {
    compares += 1;
    if (A[index] > pivot) {
        return true;
    }

    else {
        return false;
    }
}

//Returns the spot where the array will be divided.
int partition(uint32_t *arr, uint32_t lo, uint32_t hi) {

    uint32_t pivot = arr[lo + ((hi - lo) / 2)];
    int i = lo - 1;
    int j = hi + 1;

    while (i < j) {
        i += 1;
        while (compare_quick1(arr, i, pivot)) {
            i += 1;
        }

        j -= 1;

        while (compare_quick2(arr, j, pivot)) {
            j -= 1;
        }

        if (i < j) {
            moves += 3;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    return j;
}

//Instead of recursion, we are using iteration for quicksort. We push
//end indices onto the stack 2 at a time and iterate forwards for i and backwards for j
//comparing values and moving them to the correct side of the partition based on
//if they are greater or less than the pivot. Partition returns a value used as the either the
//max or min value for the next partition.
void quick_sort(uint32_t *A, uint32_t n) {

    int64_t lo = 0;
    int64_t hi = 0;

    int64_t left = 0;
    int64_t right = n - 1;

    Stack *S;
    S = stack_create();
    stack_push(S, left);
    stack_push(S, right);

    while (!stack_empty(S)) {
        stack_pop(S, (&hi));
        stack_pop(S, (&lo));
        uint32_t p = partition(A, lo, hi);
        if ((p + 1) < hi) {

            stack_push(S, p + 1);
            stack_push(S, hi);
        }

        if (lo < p) {

            stack_push(S, lo);
            stack_push(S, p);
        }
    }
}
