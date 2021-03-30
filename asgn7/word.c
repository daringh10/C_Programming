#include "word.h"

#include "code.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

//Creates a word, consisting of w->len = len and w->syms,
//a character array copied from the argument syms.
Word *word_create(uint8_t *syms, uint32_t len) {
    Word *wrd = (Word *) calloc(1, sizeof(Word));
    if (wrd) {
        wrd->len = len;
        wrd->syms = (uint8_t *) calloc(len, sizeof(uint8_t));
        if (wrd->syms) {
            for (uint32_t i = 0; i < len; i++) {
                wrd->syms[i] = syms[i];
            }
            return wrd;
        }
        return wrd;
    }
    return wrd;
}

//Creates a new word with a len of w->len + 1 and a new character at the end.
Word *word_append_sym(Word *w, uint8_t sym) {

    Word *new_word = word_create(w->syms, w->len);

    new_word->syms = realloc(new_word->syms, (w->len + 1) * sizeof(uint8_t));
    new_word->syms[w->len] = sym;
    new_word->len = w->len + 1;

    return new_word;
}

//Frees the syms array then the word.
void word_delete(Word *w) {
    free(w->syms);
    w->syms = NULL;
    free(w);
    w = NULL;
}

//Creates a wordtable, an array of word pointers. The first word is the empty word.
WordTable *wt_create(void) {
    WordTable *wt = (WordTable *) calloc(MAX_CODE, sizeof(Word *));
    if (wt) {
        wt[EMPTY_CODE] = word_create(0, 0);
        return wt;
    }
    return wt;
}

//Resets a word table so that all words but the worda at index start_code are deleted.
void wt_reset(WordTable *wt) {
    for (int i = START_CODE; i < MAX_CODE; i++) {
        if (wt[i] != NULL) {
            word_delete(wt[i]);
            wt[i] = NULL;
        }
    }
}

//Deletes every word, including the word at start_code.
void wt_delete(WordTable *wt) {
    for (int i = EMPTY_CODE; i < MAX_CODE; i++) {
        if (wt[i] != NULL) {
            word_delete(wt[i]);
            wt[i] = NULL;
        }
    }
    free(wt);
    wt = NULL;
}
