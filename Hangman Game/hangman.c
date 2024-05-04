#include "hangman_helpers.h"

#include <stdio.h>
#include <stdlib.h>
int currenterr = 0;
int numcorrect = 0;
int seclen;
char a;
int punctracker = 0;
char seccopy[257];
char guessedletters[26];
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("wrong number of arguments\n");
        printf("Usage: ./hangman word <secret word or phrase>\n");
        printf("if the secret is multiple words, you must quote it\n");
        exit(1);
    }
    strncpy(seccopy, argv[1], sizeof(seccopy));
    for (int i = 2; i < argc; i++) {
        strncat(seccopy, " ", 256 - strlen(seccopy) - 1);
        strncat(seccopy, argv[i], 256 - strlen(seccopy) - 1);
    }
    seclen = (int) strlen(seccopy);
    if (!validate_secret(seccopy)) {
        exit(1);
    }
    seccopy[seclen] = '\0';
    char tracker[seclen];
    tracker[seclen] = '\0';
    for (int i = 0; i < seclen; i++) {
        if ((seccopy[i] == ' ') || (seccopy[i] == '-') || (seccopy[i] == '\'')) {
            tracker[i] = seccopy[i];
            punctracker++;
        } else {
            tracker[i] = '_';
        }
    }
    while (currenterr <= LOSING_MISTAKE) {
        printf("%s", CLEAR_SCREEN);
        printf("%s\n\n", arts[currenterr]);
        printf("    Phrase: ");
        for (int i = 0; i < (int) strlen(tracker); i++) {
            printf("%c", tracker[i]);
        }
        printf("\nEliminated: ");
        for (int i = 0; i < 26; i++) {
            for (int j = 0; j < (int) strlen(guessedletters); j++) {
                int pos = guessedletters[j] - 'a';
                if (i == pos) {
                    printf("%c", guessedletters[j]);
                }
            }
        }
        if (currenterr == LOSING_MISTAKE) {
            printf("\n\n");
            break;
        }
        printf("\n\n");
        if (numcorrect >= seclen - punctracker) {
            printf("You win! The secret phrase was: %s\n", tracker);
            exit(0);
        }
        do {
            a = read_letter();
        } while ((string_contains_character(guessedletters, a))
                 || (string_contains_character(tracker, a)) || !(is_lowercase_letter(a)));
        if (string_contains_character(seccopy, a)) {
            for (int x = 0; x <= (int) sizeof(seccopy); x++) {
                if (a == seccopy[x]) {
                    numcorrect++;
                    tracker[x] = a;
                }
            }
            continue;
        }

        else {
            guessedletters[currenterr] = a;
            currenterr++;
        }
    }
    printf("You lose! The secret phrase was: %s\n", seccopy);
}
