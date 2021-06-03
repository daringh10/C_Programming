#include "bf.h"
#include "hash.h"
#include "parser.h"

#include <ctype.h>
#include <regex.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "mf:h:"
#define WORD    "([a-zA-Z0-9_'-]+)"

#define HT_SIZE   10000
#define BF_SIZE   1 << 20
#define BUFF_SIZE 1024

//Function to convert all input strings to lowercase.
//Not sure if theres a way to do it with regex, I looked up
//ways to do it with ?i but never could get it to work. :(
char *toLower(char *word) {
    for (uint32_t i = 0; i < strlen(word); i++) {
        int result = tolower(word[i]);
        word[i] = result;
    }
    return word;
}

//Main function
int main(int argc, char **argv) {

    //Sets the sizes of the BF and HT
    int ht_size = HT_SIZE;
    int bf_size = BF_SIZE;
    //Sets the mtf rule
    bool mtf_rule = false;

    //Parses the command line for command line arguments which
    //will alter the ht and bf
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {

        case 'h': ht_size = atoi(optarg); break;

        case 'f': bf_size = atoi(optarg); break;

        case 'm': mtf_rule = true; break;
        }
    }

    //Creates a hash table of specified size and mtf variance.
    //This will act as the main hashtable for badspeak and newspeak.
    HashTable *ht = ht_create(ht_size, mtf_rule);
    if (!ht) {
        fprintf(stderr, "Failed to create hash table.\n");
        return 0;
    }

    //Creates a bf of specified size.
    BloomFilter *bf = bf_create(bf_size);
    if (!bf) {
        fprintf(stderr, "Invalid Bloom filter size.\n");
        return 0;
    }

    //Reads in words from the badspeak.txt file and inserts them into the hash
    //table and bloom filter.
    FILE *infile_badspeak = fopen("badspeak.txt", "r");
    char badspeak[BUFF_SIZE];
    while (fscanf(infile_badspeak, "%s\n", badspeak) != EOF) {
        bf_insert(bf, badspeak);
        ht_insert(ht, badspeak, NULL);
    }
    fclose(infile_badspeak);

    //Reads in wrods from newspeak.txt and inserts them into the hash and bloom filter.
    FILE *infile_oldspeak = fopen("newspeak.txt", "r");
    char oldspeak[BUFF_SIZE];
    char newspeak[BUFF_SIZE];
    while (fscanf(infile_oldspeak, "%s %s\n", oldspeak, newspeak) != EOF) {
        bf_insert(bf, oldspeak);
        ht_insert(ht, oldspeak, newspeak);
    }
    fclose(infile_oldspeak);

    //Initiailizes the regular expressiona and compiles it.
    //Checks to see if the regular expression compiled correctly.
    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        return 1;
    }

    //Creates 2 linked lists that will hold the nodes of words that were
    //Used that were either classifed as badspeak or newspeak.
    LinkedList *badspeak_words_found;
    badspeak_words_found = ll_create(mtf_rule);
    LinkedList *oldspeak_words_found;
    oldspeak_words_found = ll_create(mtf_rule);

    //Reads stdin and checks to see if a word has three 1's set in the bloomfilter.
    //If so, then the word may be in the bloom filter. If not, then guaranteed not
    //in the bf. Also converts strings to lowercase.
    char *word;
    while ((word = next_word(stdin, &re)) != NULL) {
        word = toLower(word);
        if (bf_probe(bf, word)) {

            Node *word_found;
            word_found = ht_lookup(ht, word);

            if (word_found != NULL && word_found->newspeak == NULL) {
                ll_insert(badspeak_words_found, word_found->oldspeak, NULL);

            }

            else if (word_found != NULL && word_found->newspeak != NULL) {
                ll_insert(oldspeak_words_found, word_found->oldspeak, word_found->newspeak);
            }
        }
    }

    //These if statements print the correct statemnt depending on the words used.
    if (ll_length(badspeak_words_found) > 0 && ll_length(oldspeak_words_found) == 0) {
        printf("Dear Comrade,\n\n"

               "You have chosen to use degenerate words that may cause hurt\n"
               "feelings or cause your comrades to think unpleasant thoughts.\n"
               "This is doubleplus bad. To correct your wrongthink and\n"
               "preserve community consensus we will be sending you to joycamp\n"
               "administered by Medellin’s Miniluv. Beware of the hippos.\n\n"

               "Your errors:\n\n"

        );
        ll_print(badspeak_words_found);

    }

    else if (ll_length(badspeak_words_found) == 0 && ll_length(oldspeak_words_found) > 0) {
        printf("Dear Comrade,\n\n"

               "Submitting your text helps to preserve feelings and prevent\n"
               "badthink. Some of the words that you used are not goodspeak.\n"
               "The list shows how to turn the oldspeak words into newspeak.\n\n");
        ll_print(oldspeak_words_found);

    }

    else if (ll_length(badspeak_words_found) > 0 && ll_length(oldspeak_words_found) > 0) {
        printf("Dear Comrade,\n\n"

               "You have chosen to use degenerate words that may cause hurt\n"
               "feelings or cause your comrades to think unpleasant thoughts.\n"
               "This is doubleplus bad. To correct your wrongthink and\n"
               "preserve community consensus we will be sending you to joycamp\n"
               "administered by Medellin's Miniluv. Beware of the hippos.\n\n"

               "Your errors:\n\n"

        );
        ll_print(badspeak_words_found);
        printf("\nThink of these words on your vacation!\n\n");
        ll_print(oldspeak_words_found);
    }

    //Frees all allocated memory for the bf,ht,ll's.
    //Also frees any memory associated with regcomp.
    ll_delete(&badspeak_words_found);
    ll_delete(&oldspeak_words_found);
    ht_delete(&ht);
    bf_delete(&bf);
    regfree(&re);
}
