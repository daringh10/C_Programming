#include "set.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//Returns 0, or the empty set.
Set set_empty(void) {
    return 0;
}

//Returns whether a value is a member of a set.
bool set_member(Set s, uint8_t x) {
    return s & (0x1 << x % 32);
}
//Adds a value to a set.
Set set_insert(Set s, uint8_t x) {
    s |= (0x1 << x % 32);
    return s;
}

//Removes a value from a set.
Set set_remove(Set s, uint8_t x) {
    s &= (~(0x1 << x % 32));
    return s;
}

//Returns all the values that are the same in both sets.
Set set_intersect(Set s, Set t) {
    return s & t;
}

//Returns all values from both sets.
Set set_union(Set s, Set t) {
    return s | t;
}

//Returns all elements not in the set.
Set set_complement(Set s) {
    return ~s;
}

//Returns all the elements in set s not in set t.
Set set_difference(Set s, Set t) {

    return s & (~t);
}
