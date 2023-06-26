#include "arithmetic.h"


/*
 * This function performs addition modulo 3 using the bit encoding
*/
void addition(int *r0, int *r1, int a0, int a1, int b0, int b1) {
	// Define intermediate variables
	int t, u, v, w;

	// Perform addition
	t = a0 ^ b1;
	u = a1 ^ b0;
	v = t ^ a1;
	w = u ^ b1;
	*r0 = v | w;
	*r1 = t & u;
}

/*
 * This function performs subtraction modulo 3 using the bit encoding
*/
void subtraction(int *r0, int *r1, int a0, int a1, int b0, int b1) {
	// Define intermediate variable
	int t;

	// Perform subtraction
	t = a0 ^ b0;
	*r0 = t | (a1 ^ b1);
	*r1 = (t ^ b1) & (b0 ^ a1);
}

/*
 * This function performs negation modulo 3 using the bit encoding
*/
void negation(int *r0, int *r1, int a0, int a1) {
	*r0 = a0;
	*r1 = a0 ^ a1;
}

/*
 * This function performs squaring modulo 3 using the bit encoding
*/
void squaring(int *r0, int *r1, int a0, int a1) {
	*r0 = a0;
	*r1 = 0;
}

/*
 * This function performs addition with a square modulo 3 using the bit encoding
*/
void square_and_add(int *r0, int *r1, int a0, int a1, int b0) {
	// Define intermediate variables
	int u, v;

	// Perform addition with square
	u = a1 ^ b0;
	v = a0 ^ a1;
	*r0 = v | u;
	*r1 = a0 & u;
}	

/*
 * This function mimics the log2 function when inputs are not 0
*/
float logarithm(float input, int base) {
	if (input) {
		return (float) log(input) / (float) log(base);
	}
	else {
		return 0;
	}
}

