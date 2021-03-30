#include "mathlib.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "asctel" //Command line options

//Code from Eugene, from section
//To make things less cluttered, initialize bools to aid with
//Dealing -a, so there is no repeats
bool all, sin_check, cos_check, tan_check, exp_check, log_check = 0;

int main(int argc, char **argv) {

    //If only mathlib-test is executed with no arguments,
    //Show the user the various command line arguments
    //*Formatted to match test binary*
    if (argc < 2) {
        fprintf(stderr, "Program usage: %s -[asctel]\n", argv[0]);
        printf("  -a           runs all tests (sin, cos, tan, exp, log)\n");
        printf("  -s           runs sin tests\n");
        printf("  -c           runs cos tests\n");
        printf("  -t           runs tan tests\n");
        printf("  -e           runs exp tests\n");
        printf("  -l           runs log tests\n");
    }

    //Code from Eugene, from section
    //Initilaize an integer opt that corresponds to different command line input
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {

        switch (opt) {

        case 'a': all = 1; break;

        case 's': sin_check = 1; break;

        case 'c': cos_check = 1; break;

        case 't': tan_check = 1; break;

        case 'e': exp_check = 1; break;

        case 'l': log_check = 1; break;

        default: fprintf(stderr, "Program usage: %s -[asctel]\n", argv[0]);
        }
    }

    //This line takes into acount if just -a was entered
    if (all == 1) {
        sin_check = 1;
        cos_check = 1;
        tan_check = 1;
        exp_check = 1;
        log_check = 1;
    }

    //Each if statement prints the correct assigned format
    if (sin_check == 1) {
        printf("  x            Sin              Library        Difference\n");
        printf("  -            ---              -------        ----------\n");
        for (double i = (-2) * M_PI; i < 2 * M_PI; i += .1) {
            printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n", i, Sin(i), sin(i), Sin(i) - sin(i));
        }
    }

    if (cos_check == 1) {
        printf("  x            Cos              Library        Difference\n");
        printf("  -            ---              -------        ----------\n");
        for (double i = (-2) * M_PI; i <= 2 * M_PI; i += .1) {
            printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n", i, Cos(i), cos(i), Cos(i) - cos(i));
        }
    }

    if (tan_check == 1) {
        printf("  x            Tan              Library        Difference\n");
        printf("  -            ---              -------        ----------\n");
        for (double i = (-1) * (M_PI / 3); i < (M_PI / 3); i += .1) {
            printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n", i, Tan(i), tan(i), Tan(i) - tan(i));
        }
    }

    if (exp_check == 1) {
        printf("  x            Exp              Library        Difference\n");
        printf("  -            ---              -------        ----------\n");
        for (double i = 1; i < 10; i += .1) {
            printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n", i, Exp(i), exp(i), Exp(i) - exp(i));
        }
    }

    if (log_check == 1) {
        printf("  x            Log              Library        Difference\n");
        printf("  -            ---              -------        ----------\n");
        for (double i = 1; i < 10; i += .1) {
            printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n", i, Log(i), log(i), Log(i) - log(i));
        }
    }
    return 0;
}
