#ifndef AVALANCHE_H
#define AVALANCHE_H

#include "ternary.h"

// Function declarations
void generate_avalanche_probability_vector(float *avalanche_probability, int input_difference, int nr_of_samples, int nr_of_rounds);
void initialize_random_state(state s_random);
void initialize_difference_state(state s_difference, state s_random, int input_difference);
void update_avalanche_probability(float *avalanche_probability, state s_random, state s_difference, int nr_of_samples);
float get_avalanche_entropy(float *avalanche_probability);
float get_avalanche_weight(float *avalanche_probability);
int get_avalanche_dependence(float *avalanche_probability);


#endif

