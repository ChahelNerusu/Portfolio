#include "stack.h"

#include "messages.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
double stack[STACK_CAPACITY];
int stack_size = 0;

bool stack_push(double item) {
    if (stack_size < STACK_CAPACITY) {
        stack[stack_size] = item;
        stack_size++;
        return true;
    } else {
        fprintf(stderr, ERROR_NO_SPACE, item);
        fprintf(stderr, "> ");
        exit(0);
        return false;
    }
    return 0;
}

bool stack_peek(double *item) {
    if (stack_size == 0) {
        return false;
        exit(0);
    } else {
        *item = stack[stack_size - 1];
        return true;
    }
}

bool stack_pop(double *item) {
    if (stack_size == 0) {
        return false;
    } else {
        --stack_size;
        *item = stack[stack_size];
        return true;
    }
}

void stack_clear(void) {
    stack_size = 0;
}

void stack_print(void) {
    if (stack_size == 0) {
        return;
    }
    printf("%.10f", stack[0]);
    for (int i = 1; i < stack_size; i++) {
        printf(" %.10f", stack[i]);
    }
}
