#ifndef __OPERATIONS_H__
#define __OPERATIONS_H__

#include "operators.h"

#include "mathlib.h"
#include "messages.h"
#include "stack.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef double (*binary_operator_fn)(double lhs, double rhs);

typedef double (*unary_operator_fn)(double x);

double operator_add(double lhs, double rhs) {
    double result;
    result = lhs + rhs;
    return result;
}

double operator_sub(double lhs, double rhs) {
    double result;
    result = lhs - rhs;
    return result;
}

double operator_mul(double lhs, double rhs) {
    double result;
    result = lhs * rhs;
    return result;
}

double operator_div(double lhs, double rhs) {
    double result;
    result = lhs / rhs;
    return result;
}

bool parse_double(const char *s, double *d) {
    char *endptr;
    double result = strtod(s, &endptr);
    if (endptr != s) {
        *d = result;
        return true;
    } else {
        return false;
    }
}

bool apply_binary_operator(binary_operator_fn op) {
    double rhs;
    double lhs;
    if (stack_size > 1) {
        stack_pop(&rhs);
        stack_pop(&lhs);
        double result = op(lhs, rhs);
        stack_push(result);
        return true;
    } else {
        fprintf(stderr, "%s", ERROR_BINARY_OPERATOR);
        return false;
    }
}

bool apply_unary_operator(unary_operator_fn op) {
    if (stack_size < 1) {
        fprintf(stderr, "%s", ERROR_UNARY_OPERATOR);
        return false;
    }
    double x;
    assert(stack_pop(&x));
    double result = op(x);
    assert(stack_push(result));
    return true;
}
#endif
