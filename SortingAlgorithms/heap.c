
#include "heap.h"

#include "stack.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//Initializes external variables
//for counting moves and compares.
extern uint64_t moves;
extern uint64_t compares;

bool compare_heap_check(uint32_t *A, uint32_t left, uint32_t right) {
    compares += 1;
    if ((A[right - 1] > A[left - 1])) {

        return true;
    }

    else {
        return false;
    }
}

//Returns which child is greatest. Only 2 options right or left.
int max_child(uint32_t *arr, uint32_t first, uint32_t last) {

    uint32_t left = 2 * first;
    uint32_t right = left + 1;

    if ((right <= last) && compare_heap_check(arr, left, right)) {

        return right;
    }

    return left;
}

//Creates the max heap. Largest element is moved to the beginning.
void fix_heap(uint32_t *arr, uint32_t first, uint32_t last) {

    bool found = false;
    uint32_t parent = first;
    uint32_t great = max_child(arr, parent, last);

    while ((parent <= (last / 2)) && (found == false)) {
        compares += 1;
        if ((arr[parent - 1]) < (arr[great - 1])) {

            uint32_t temp = arr[parent - 1];
            arr[parent - 1] = arr[great - 1];
            arr[great - 1] = temp;
            moves += 3;
            parent = great;
            great = max_child(arr, parent, last);
        }

        else {
            found = true;
        }
    }
}

//Creates the array such that is obeys the rules of  a heap.
//If k is the current index, then children will always be either
//2k or 2k+1 indices away. To get the parent, k/2 will get achieve this
//for any child.
void build_heap(uint32_t *arr, uint32_t first, uint32_t last) {
    for (uint32_t parent = (last / 2); parent > first - 1; parent -= 1) {
        fix_heap(arr, parent, last);
    }
}

//Everytime an item is moved the heap must be fixed, which is why we call
//it every swap.
void heap_sort(uint32_t *A, uint32_t n) {

    uint32_t first = 1;
    uint32_t last = n;
    build_heap(A, first, last);

    for (uint32_t leaf = last; leaf > first; leaf -= 1) {
        uint32_t temp = A[first - 1];
        A[first - 1] = A[leaf - 1];
        A[leaf - 1] = temp;
        moves += 3;
        fix_heap(A, first, leaf - 1);
    }
}
