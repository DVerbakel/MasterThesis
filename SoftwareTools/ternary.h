#ifndef TERNARY_H
#define TERNARY_H

#include <inttypes.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>


/*
 * We represent the 3 x 64 state containing either 0, 1, or 2 as a 6 x 64 state
 *  ----------------------------------------------------------------
 * | s_(0,2)   s_(1,2)   s_(2,2)   s_(3,2)   ...                    | state[0]
 * |   128       65         2        131     ...                    | state[1]
 *  ----------------------------------------------------------------
 * | s_(0,1)   s_(1,1)   s_(2,1)   s_(3,1)   ...                    | state[2]
 * |   64         1        130       67      ...                    | state[3]
 *  ----------------------------------------------------------------
 * | s_(0,0)   s_(1,0)   s_(2,0)   s_(3,0)   ...                    | state[4]
 * |    0        129       66         3      ...                    | state[5]
 *  ----------------------------------------------------------------
 * Note that:
 * - If the y-coordinate is 0, the bits are stored in state[4] and state[5]
 * - If the y-coordinate is 1, the bits are stored in state[2] and state[3]
 * - If the y-coordinate is 2, the bits are stored in state[0] and state[1]
 * Examples: 
 * - The 0th trit (0/1/2) is encoded in state[0][0] and state[1][0]
 * - The 129th trit is encoded in state[2][1] and state[3][1]
 * - The 191th trit is encoded in state[4][63] and state[5][63]
 * We apply the following encoding:
 * - 0 is encoded using 0 & 0
 * - 1 is encoded using 1 & 0
 * - 2 is encoded using 1 & 1
 * - the encoding 0 & 1 does not exist: if encountered it indicates an error 
*/
typedef uint64_t state[6];

// Define the parameters for t, g, r0, r1 and r2
#define PARAM_T 6
#define PARAM_TP 5
#define PARAM_G 7
#define PARAM_R0 0
#define PARAM_R1 1
#define PARAM_R2 24

// Function declarations
void theta_step_four_terms(state s);
void theta_step_three_terms(state s);
void rho_step(state s);
void gamma_step(state s);
void round_function_one(state s);
void round_function_two(state s);


#endif

