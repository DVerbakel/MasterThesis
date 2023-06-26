#include "util.h"


/* 
 * This function copies the src_state to the dest_state
*/ 
void copy_state(state src_state, state dest_state) {
	for (int i = 0; i < 6; i++) {
		dest_state[i] = src_state[i];
	}
}

/*
 * This function sets the first bit according to r0 and the second bit according to r1
*/
void set_bits(int r0, int r1, int x, int *y, state s) {
	// if r0 equals to 1, we set the first resulting bit in the state to 1, otherwise to 0
	if (r0) { 
		s[y[0]] |= (1UL << (63-x));
	}
	else {
		s[y[0]] &= ~(1UL << (63-x));
	}
	// if r1 equals to 1, we set the second resulting bit in the state to 1, otherwise to 0
	if (r1) { 
		s[y[1]] |= (1UL << (63-x));
	}
	else {
		s[y[1]] &= ~(1UL << (63-x));
	}
}

