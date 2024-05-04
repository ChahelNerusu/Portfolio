#include "hangman_helpers.h"

#include <stdbool.h>
#include <stdlib.h>

bool is_lowercase_letter(char c) {
    if ('a' <= c && c <= 'z') {
        return true;
    } else {
        return false;
    }
    return 0;
}

bool validate_secret(const char *secret) {
    bool yes = true;
    if ((int) strlen(secret) <= 256) {
        for (int i = 0; i < (int) strlen(secret); i++) {
            if (!is_lowercase_letter(secret[i]) && secret[i] != ' ' && secret[i] != '-'
                && secret[i] != '\'') {
                printf("invalid character: '%c'\n", secret[i]);
                printf("the secret phrase must contain only lowercase letters, spaces, hyphens, "
                       "and apostrophes\n");
                yes = false;
                break;
            }
        }

    } else {
        printf("the secret phrase is over 256 characters\n");
        yes = false;
    }
    return yes;
    return 0;
}

bool string_contains_character(const char *s, char c) {
    bool yes = false;
    for (int i = 0; i < (int) strlen(s); i++) {
        if (c == s[i]) {
            yes = true;
            break;
        }
    }
    return yes;
    return 0;
}

char read_letter(void) {
    int a;
    printf("Guess a letter: ");
    a = getchar();
    while (a == '\n') {
        a = getchar();
    }
    if (a == EOF) {
        exit(1);
    }
    return (char) a;
}
