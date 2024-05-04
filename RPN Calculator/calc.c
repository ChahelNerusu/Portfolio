#include "mathlib.h"
#include "messages.h"
#include "operators.h"
#include "stack.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define OPTIONS "mh"
int main(int argc, char **argv) {
    int opt;
    int swval = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h':
            printf(USAGE, argv[1]);
            exit(0);
            break;
        case 'm': swval = 1; break;
        default:
            printf(USAGE, argv[0]);
            exit(0);

            break;
        }
    }
    while (1) {
        stack_clear();
        fprintf(stderr, "> ");
        char *saveptr;
        char expr[1024];
        bool error = false;
        if (fgets(expr, sizeof(expr), stdin) == NULL) {
            exit(0);
        }
        if (expr[(int) strlen(expr) - 1] == '\n') {
            expr[(int) strlen(expr) - 1] = '\0';
        }
        const char *token = strtok_r(expr, " ", &saveptr);
        while (token != NULL && !error) {
            double num;
            if (strcmp(token, "+") == 0) {
                error = !apply_binary_operator(operator_add);
            } else if (strcmp(token, "-") == 0) {
                error = !apply_binary_operator(operator_sub);
            } else if (strcmp(token, "/") == 0) {
                error = !apply_binary_operator(operator_div);
            } else if (strcmp(token, "*") == 0) {
                error = !apply_binary_operator(operator_mul);
            } else if (strcmp(token, "%") == 0) {
                error = !apply_binary_operator(fmod);
            } else if (strcmp(token, "t") == 0 && swval == 0) {
                error = !apply_unary_operator(Tan);
            } else if (strcmp(token, "s") == 0 && swval == 0) {
                error = !apply_unary_operator(Sin);
            } else if (strcmp(token, "c") == 0 && swval == 0) {
                error = !apply_unary_operator(Cos);
            } else if (strcmp(token, "r") == 0 && swval == 0) {
                error = !apply_unary_operator(Sqrt);
            } else if (strcmp(token, "a") == 0 && swval == 0) {
                error = !apply_unary_operator(Abs);
            } else if (strcmp(token, "t") == 0 && swval == 1) {
                error = !apply_unary_operator(tan);
            } else if (strcmp(token, "s") == 0 && swval == 1) {
                error = !apply_unary_operator(sin);
            } else if (strcmp(token, "c") == 0 && swval == 1) {
                error = !apply_unary_operator(cos);
            } else if (strcmp(token, "r") == 0 && swval == 1) {
                error = !apply_unary_operator(sqrt);
            } else if (strcmp(token, "a") == 0 && swval == 1) {
                error = !apply_unary_operator(fabs);
            } else if (parse_double(token, &num)) {
                stack_push(num);
            } else {
                error = true;
                if ((int) strlen(token) < 2) {
                    fprintf(stderr, ERROR_BAD_CHAR, *token);
                } else {
                    fprintf(stderr, ERROR_BAD_STRING, token);
                }
            }

            token = strtok_r(NULL, " ", &saveptr);
        }
        if (error == false) {
            stack_print();
            stack_clear();
            printf("\n");
        }
    }
    return 0;
}
