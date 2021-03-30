#include "names.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SEEDMAX 2147483647

u_int32_t left(u_int32_t pos, u_int32_t players) {
    return (pos + players - 1) % players;
}

u_int32_t right(u_int32_t pos, u_int32_t players) {
    return (pos + 1) % players;
}

int main(void) {

    /* Initializes number of players and starting seed */
    u_int32_t n = 0;
    u_int32_t gameSeed = 0;

    /* While loop will run until int beteween 2 and 10 is inputted */
    while (1) {
        /* Asks user to input integer, and scans  into n */
        printf("Number of players: ");
        scanf("%d", &n);
        /* If input is valid then break */
        if (n >= 2 && n <= 10) {
            break;
        }
        /* If input is anything other than correct then continue asking */
        else {
            fprintf(stderr, "Invalid number of players.\n");
        }
        while (getchar() != '\n')
            ;
    }

    while (1) {
        printf("Random gameSeed: ");
        scanf("%d", &gameSeed);

        if (gameSeed >= 0 && gameSeed <= SEEDMAX) {
            break;
        }

        else {
            fprintf(stderr, "Invalid random seed\n");
        }
        while (getchar() != '\n')
            ;
    }

    /* Initializes an array of length n */
    u_int32_t lives[n];

    /* Initalizes counter for alive vampires in a round */
    u_int32_t aliveCount;

    /*Iterator for parsing lives array */
    u_int32_t i;

    /*Iterator for checking "who is alive" loop */
    u_int32_t j;

    /* Loop to set every vampire life to 3 */
    for (i = 0; i < n; ++i) {
        lives[i] = 3;
    }

    u_int32_t roundCounter = 0;

    /* calling srand with the argument user input Seed */
    srand(gameSeed);

    /*!!Game Loop!! */
    while (1) {

        /* Array to store vampire location in circle and roll */
        u_int32_t lifeSteal[2] = { 0, 20 };

        /*Resets vampire alive counter for the round */
        aliveCount = 0;

        /* Loop through lives array to check what vampires are at 1 or more lives */
        for (j = 0; j < n; ++j) {
            /*Incriments aliveCount for vampires at 1 or more lives */
            if (lives[j] > 0) {
                ++aliveCount;
            }
        }

        if (aliveCount == 1) {
            for (j = 0; j < n; ++j) {
                if (lives[j] >= 1) {
                    printf("%s wins the Garlic Game!\n", names[j]);
                    break;
                }
            }
            break;
        }

        /*Prints the rounder number */
        printf("Round %d\n", roundCounter);

        for (i = 0; i < n; ++i) {
            if (lives[i] == 0) {
                continue;
            } else {

                u_int32_t first = rand() % 6;
                u_int32_t second = rand() % 6;
                u_int32_t diceRoll = first + second;

                printf(" - %s ", names[i]);
                printf("rolls %s...", rolls[first][second]);

                if (rolls[first][second] == rolls[5][5]) {

                    if (lives[left(i, n)] == 0) {
                        printf(" resurrects %s", names[left(i, n)]);
                    } else {
                        printf(" sparkles %s", names[left(i, n)]);
                    }

                    if (lives[right(i, n)] == 0) {
                        printf(" resurrects %s", names[right(i, n)]);
                    } else {
                        printf(" sparkles %s", names[right(i, n)]);
                    }

                    lives[left(i, n)] += 1;
                    lives[right(i, n)] += 1;
                }

                printf("\n");

                if (diceRoll < lifeSteal[1]) {
                    lifeSteal[0] = i;
                    lifeSteal[1] = diceRoll;
                }
            }
        }

        lives[lifeSteal[0]] -= 1;
        printf("%s is forced to eat garlic!\n", names[lifeSteal[0]]);

        if (lives[lifeSteal[0]] == 1) {
            printf("%s has 1 life remaining.\n", names[lifeSteal[0]]);
        }

        else if (lives[lifeSteal[0]] == 0) {
            printf("%s has died.\n", names[lifeSteal[0]]);
        }

        else {
            printf("%s has %d lives remaining.\n", names[lifeSteal[0]], lives[lifeSteal[0]]);
        }

        roundCounter += 1;
    }
}
