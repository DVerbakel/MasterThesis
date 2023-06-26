#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include "ternary.h"

// Function declarations
void addition(int *r0, int *r1, int a0, int a1, int b0, int b1);
void subtraction(int *r0, int *r1, int a0, int a1, int b0, int b1);
void negation(int *r0, int *r1, int a0, int a1);
void squaring(int *r0, int *r1, int a0, int a1);
void square_and_add(int *r0, int *r1, int a0, int a1, int b0);
float logarithm(float input, int base);


#endif

