#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum faciem { LEFT, RIGHT, CENTER, PASS } faces;
faces die[] = {LEFT, RIGHT, CENTER, PASS, PASS, PASS};

int main(void) {
    
    int seed;
    printf("Random seed: ");
    scanf("%d", &seed);
    srand(seed);
    for (int i = 0; i < 20; i += 1) {
        int roll = rand() % 6;
        printf("random = %d, face = %d\n", roll, die[roll]);
    }
    return 0;
}




//
 // Returns the position of the player to the left .//
// pos: The position of the current player .
 // players : The number of players in the game .
 //
 uint32_t left ( uint32_t pos , uint32_t players ) {
 	return ( pos + players - 1) % players ;
 }

 //
 // Returns the position of the player to the right .
 //
 // pos: The position of the current player .
 // players : The number of players in the game .
 //
 uint32_t right ( uint32_t pos , uint32_t players ) {
 return ( pos + 1) % players ;
 }
