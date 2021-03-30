#include "stack.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//Defines the struct Stack
struct Stack {
    uint32_t top;
    uint32_t capacity;
    int64_t *items;
};

//Creating our stack.
Stack *stack_create(void) {
    //Allocates memory for a stack.
    Stack *s = (Stack *) (calloc(1, sizeof(Stack)));

    //Sets the top value to the first spot in the array
    //And sets the min # of items to min_capacity
    s->top = 0;
    s->capacity = MIN_CAPACITY;

    s->items = (int64_t *) (calloc(s->capacity, sizeof(int64_t)));

    return s;
}

//Frees the array items, then the stack struc itself, then sets to null.
void stack_delete(Stack **s) {
    free((*s)->items);
    free(*s);
    *s = NULL;
}

//If top == 0 then we know the index of the stack is at the
//very beginning.
bool stack_empty(Stack *s) {
    return s->top == 0;
}

//Puts x in the current position of top
//then incriments top by 1
bool stack_push(Stack *s, int64_t x) {
    if (s->top == s->capacity) {
        s->capacity *= 2;
        s->items = realloc(s->items, (s->capacity * (sizeof(int64_t))));
        if (s->items == NULL) {
            return false;
        }
    }
    s->items[s->top] = x;
    s->top += 1;
    return true;
}

//Pops off the current item under where top is.
bool stack_pop(Stack *s, int64_t *x) {
    if (stack_empty(s)) {
        return false;
    }

    s->top -= 1;
    *x = s->items[s->top];
    s->items[s->top] = 0;
    return true;
}

//Helpful
void stack_print(Stack *s) {
    for (int64_t i = 0; i < s->top; i++) {
        printf("%ld", s->items[i]);
    }
}
