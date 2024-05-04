#include "mathlib.h"

#include "stack.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
double Abs(double x) {
    double result;
    if (x < 0) {
        result = (x / -1);
    } else {
        result = x;
    }
    return result;
}

double Sqrt(double x) {
    if (x < 0) {
        return nan("nan");
    }
    double old = 0.0;
    double new = 1.0;
    while (Abs(old - new) > EPSILON) {
        old = new;
        new = 0.5 * (old + (x / old));
    }
    return new;
}

double Sin(double x) {
    while (x < 0) {
        x += 2 * M_PI;
    }
    while (x > 2 * M_PI) {
        x -= 2 * M_PI;
    }
    double result = x;
    double term = x;
    double x_squared = x * x;
    int n = 1;
    while (fabs(term) > EPSILON) {
        term *= (-1) * x_squared / ((2 * n) * (2 * n + 1));
        result += term;
        n++;
    }

    return result;
}

double Cos(double x) {
    while (x < 0) {
        x += 2 * M_PI;
    }
    while (x > 2 * M_PI) {
        x -= 2 * M_PI;
    }
    double result = 1.0;
    double term = 1.0;
    double x_squared = x * x;

    int n = 1;
    while (fabs(term) > EPSILON) {
        term *= (-1) * x_squared / ((2 * n - 1) * (2 * n));
        result += term;
        n++;
    }

    return result;
}
double Tan(double x) {
    x = Sin(x) / Cos(x);
    return x;
}
