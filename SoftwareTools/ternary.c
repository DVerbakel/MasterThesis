#include "ternary.h"
#include "arithmetic.h"
#include "util.h"


/* 
 * The theta function has the purpose of mixing
 * The following is computed: s_(x,y) <- s_(x,y) + s_(x+1,y+1) + s_(x+2,y+2) + s_(x+t,y+t)
 * Note that the x-coordinate should be taken modulo 64 and the y-coordinate modulo 3
*/
void theta_step_four_terms(state s) {
	// Define all variables used in the theta function
	int x, a0, a1, b0, b1, c0, c1, d0, d1, r0, r1;
	int y[2];
	state s_original;

	// Copy the state
	copy_state(s, s_original);

	// For each entry in the state	
	for (int i = 0; i < 192; i++) {
		// Define the x and y coordinate of the result (i.e. s_(x,y))
		x = i % 64;
		y[0] = 4 - 2 * (i % 3);
		y[1] = 5 - 2 * (i % 3);
		
		// Get the separate bits of a + b + c + d
		a0 = (int) ((s_original[4 - 2 * (i % 3)] >> (63-(i % 64))) & 1);
		a1 = (int) ((s_original[5 - 2 * (i % 3)] >> (63-(i % 64))) & 1);
		b0 = (int) ((s_original[4 - 2 * ((i+1) % 3)] >> (63-((i+1) % 64))) & 1);
		b1 = (int) ((s_original[5 - 2 * ((i+1) % 3)] >> (63-((i+1) % 64))) & 1);
		c0 = (int) ((s_original[4 - 2 * ((i+2) % 3)] >> (63-((i+2) % 64))) & 1);
		c1 = (int) ((s_original[5 - 2 * ((i+2) % 3)] >> (63-((i+2) % 64))) & 1);
		d0 = (int) ((s_original[4 - 2 * ((i+PARAM_T) % 3)] >> (63-((i+PARAM_T) % 64))) & 1);
		d1 = (int) ((s_original[5 - 2 * ((i+PARAM_T) % 3)] >> (63-((i+PARAM_T) % 64))) & 1);

		// Reset the result {r0,r1} and perform s_(x,y) + s_(x+1,y+1) + s_(x+2,y+2) + s_(x+t,y+t)
		r0 = 0;
		r1 = 0;
		addition(&r0, &r1, a0, a1, b0, b1);
		addition(&r0, &r1, c0, c1, r0, r1);
		addition(&r0, &r1, d0, d1, r0, r1);

		// Set s_(x,y) to the result obtained
		set_bits(r0, r1, x, y, s);
	}
}

/* 
 * The theta function has the purpose of mixing
 * The following is computed: s_(x,y) <- s_(x,y) + 2s_(x+1,y+1) + s_(x+t,y+t)
 * Note that the x-coordinate should be taken modulo 64 and the y-coordinate modulo 3
*/
void theta_step_three_terms(state s) {
	// Define all variables used in the theta function
	int x, a0, a1, b0, b1, c0, c1, r0, r1;
	int y[2];
	state s_original;

	// Copy the state
	copy_state(s, s_original);

	// For each entry in the state	
	for (int i = 0; i < 192; i++) {
		// Define the x and y coordinate of the result (i.e. s_(x,y))
		x = i % 64;
		y[0] = 4 - 2 * (i % 3);
		y[1] = 5 - 2 * (i % 3);
	
		// Get the separate bits of a + b + c
		a0 = (int) ((s_original[4 - 2 * (i % 3)] >> (63-(i % 64))) & 1);
		a1 = (int) ((s_original[5 - 2 * (i % 3)] >> (63-(i % 64))) & 1);
		b0 = (int) ((s_original[4 - 2 * ((i+1) % 3)] >> (63-((i+1) % 64))) & 1);
		b1 = (int) ((s_original[5 - 2 * ((i+1) % 3)] >> (63-((i+1) % 64))) & 1);
		c0 = (int) ((s_original[4 - 2 * ((i+PARAM_TP) % 3)] >> (63-((i+PARAM_TP) % 64))) & 1);
		c1 = (int) ((s_original[5 - 2 * ((i+PARAM_TP) % 3)] >> (63-((i+PARAM_TP) % 64))) & 1);

		// Reset the result {r0,r1} and perform s_(x,y) + 2s_(x+1,y+1) + s_(x+t,y+t)
		r0 = 0;
		r1 = 0;
		subtraction(&r0, &r1, a0, a1, b0, b1);
		addition(&r0, &r1, c0, c1, r0, r1);		

		// Set s_(x,y) to the result obtained
		set_bits(r0, r1, x, y, s);
	}
}

/*
 * The rho function has the purpose of dispersion
 * The following is computed: s_(x,y) <- s_(x+ry,y) where ry <- {r0, r1, r2}
 * Note that the x-coordinate should be taken modulo 64 and the y-coordinate modulo 3
*/
void rho_step(state s) {
	// Define the offsets r0, r1 and r2
	int offsets[3] = {
		PARAM_R2,	// used on state[0] and state[1]
		PARAM_R1,	// used on state[2] and state[3]
		PARAM_R0	// used on state[4] and state[5]
	};

	// Left rotate the 0th row with r0, the 1st row with r1 and the 2nd row with r2
	for (int i = 0; i < 3; i++) {
		s[2*i] = (s[2*i] << offsets[i]) | (s[2*i] >> (64-offsets[i]));
		s[2*i+1] = (s[2*i+1] << offsets[i]) | (s[2*i+1] >> (64-offsets[i]));
	}
}

/*
 * The gamma function has the purpose of non-linearity
 * The following is computed: s_(x,y) <- s_(x,y) + (s_(x+g,y+g))^2
 * Note that the x-coordinate should be taken modulo 64 and the y-coordinate modulo 3
*/
void gamma_step(state s) {
	// Define all variables used in the gamma function
	int x, a0, a1, b0, r0, r1;
	int y[2];
	state s_original;

	// Copy the state
	copy_state(s, s_original);

	// For each entry in the state
	for (int i = 0; i < 192; i++) {
		// Define the x and y coordinate of the result (i.e. s_(x,y))
		x = i % 64;
		y[0] = 4 - 2 * (i % 3);
		y[1] = 5 - 2 * (i % 3);
	
		// Get the separate bits of a + b
		a0 = (int) ((s_original[4 - 2 * (i % 3)] >> (63-(i % 64))) & 1);
		a1 = (int) ((s_original[5 - 2 * (i % 3)] >> (63-(i % 64))) & 1);
		b0 = (int) ((s_original[4 - 2 * ((i+PARAM_G) % 3)] >> (63-((i+PARAM_G) % 64))) & 1);
		
		// Reset the result {r0,r1} and perform s_(x,y) + (s_(x+g,y+g))^2 for (x,y) =/= (0,0)
		r0 = 0;
		r1 = 0;
		square_and_add(&r0, &r1, a0, a1, b0);		

		// Set s_(x,y) to the result obtained
		set_bits(r0, r1, x, y, s);
	}
}


/* 
 * This function performs the full round function using a ternary permutation
 * Here, theta with four terms is used
*/
void round_function_one(state s) {
	theta_step_four_terms(s);
	rho_step(s);
	gamma_step(s);
}

/* 
 * This function performs the full round function using a ternary permutation
 * Here, theta with three terms is used
*/
void round_function_two(state s) {
	theta_step_three_terms(s);
	rho_step(s);
	gamma_step(s);
}

