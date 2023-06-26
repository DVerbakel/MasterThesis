#include "avalanche.h"
#include "arithmetic.h"
#include "util.h"


/*
 * This function computes the avalanche probability vector with an input difference at bit_nr after nr_of_rounds rounds
 * Note that this generation uses nr_of_samples samples (where nr_of_samples < 3^192)
 * The avalanche probability vector is constructed as p_(3,i,1,Delta) || p_(3,i,2,Delta)
 * We will compute p_(3,i,0,Delta) as 1 - p_(3,i,1,Delta) - p_(3,i,2,Delta)
*/
void generate_avalanche_probability_vector(float *avalanche_probability, int input_difference, int nr_of_samples, int nr_of_rounds) {
	// Define a variable for the random and difference states
	state s_random;
	state s_difference;

	// Initialize the random number generator with seed 13
	srand(13);

	// For all samples
	for (int i = 0; i < nr_of_samples; i++) {
		// Initialize the random state s and difference state s+delta
		initialize_random_state(s_random);
		initialize_difference_state(s_difference, s_random, input_difference);

		// Perform a certain number of round functions on both states
		for (int j = 0; j < nr_of_rounds; j++) {
			round_function_two(s_random);
			round_function_two(s_difference);
		}

		// Calculate the difference and update the avalanche probability accordingly
		update_avalanche_probability(avalanche_probability, s_random, s_difference, nr_of_samples);
	}
	
}

/*
 * This function initializes the random state which is defined as s
*/
void initialize_random_state(state s_random) {
	// Define variables for the x and y coordinate of a certain position of the state and for the random state without encoding
	int rough_state[192] = {0};
	int x_coordinate;
	int y_coordinate[2];

	// Initialize the random state to 0
	memset(s_random, 0, sizeof(state));	

	// For each entry in the state
	for (int i = 0; i < 192; i++) {
		// Set the x and y coordinates and get a random 0, 1 or 2
		x_coordinate = i % 64;
		y_coordinate[0] = 4 - 2 * (i % 3);
		y_coordinate[1] = 5 - 2 * (i % 3);
		rough_state[i] = (rand() % 3);

		// If a random 0 was chosen, set (x,y) to 0 & 0 (encoding for 0)
		if (rough_state[i] == 0) {
			s_random[y_coordinate[0]] &= ~(1UL << (63-x_coordinate));	// set to 0
			s_random[y_coordinate[1]] &= ~(1UL << (63-x_coordinate));	// set to 0
		}
		// If a random 1 was chosen, set (x,y) to 1 & 0 (encoding for 1)
		else if (rough_state[i] == 1) {
			s_random[y_coordinate[0]] |= (1UL << (63-x_coordinate));	// set to 1
			s_random[y_coordinate[1]] &= ~(1UL << (63-x_coordinate));	// set to 0
		}
		// If a random 2 was chosen, set (x,y) to 1 & 1 (encoding for 2)
		else if (rough_state[i] == 2) {
			s_random[y_coordinate[0]] |= (1UL << (63-x_coordinate));	// set to 1
			s_random[y_coordinate[1]] |= (1UL << (63-x_coordinate));	// set to 1
		}
		else {
			printf("ERROR: something went wrong, please check the random integer generation\n");
		}
	}
}

/*
 * This function initializes the difference state which is defined as s+Delta
*/
void initialize_difference_state(state s_difference, state s_random, int input_difference) {
	// Define all variables used to generate the difference state
	int x = input_difference % 64;
	int y[2] = {4 - 2 * (input_difference % 3), 5 - 2 * (input_difference % 3)};
	int r0 = 0;
	int r1 = 0;
	int a0 = (int) ((s_random[y[0]] >> (63-x)) & 1);
	int a1 = (int) ((s_random[y[1]] >> (63-x)) & 1);

	// Initialize s_difference with what s_random is
	copy_state(s_random, s_difference);

	// Add the difference to the input_difference position
	addition(&r0, &r1, a0, a1, 1, 0);
	
	// Set s_(x,y) to the result obtained
	set_bits(r0, r1, x, y, s_difference);
}

/*
 * This function updates the avalanche probability vector according to whether a position is 0, 1 or 2
*/
void update_avalanche_probability(float *avalanche_probability, state s_random, state s_difference, int nr_of_samples) {
	// Define all variables used in the updating function
	int x, a0, a1, b0, b1, r0, r1;
	int y[2];

	// For each entry in the state
	for (int i = 0; i < 192; i++) {
		// Define the x and y coordinate of the bit we are currently looking at (i.e. s_(x,y))
		x = i % 64;
		y[0] = 4 - 2 * (i % 3);
		y[1] = 5 - 2 * (i % 3);

		// Get the separate bits of a - b
		a0 = (int) ((s_random[y[0]] >> (63-x)) & 1);
		a1 = (int) ((s_random[y[1]] >> (63-x)) & 1);
		b0 = (int) ((s_difference[y[0]] >> (63-x)) & 1);
		b1 = (int) ((s_difference[y[1]] >> (63-x)) & 1);

		// Reset the result {r0,r1} and calculate the difference between the bit in s_random and in s_difference
		r0 = 0;
		r1 = 0;
		subtraction(&r0, &r1, a0, a1, b0, b1);

		// If the difference is equal to 1, update p_(3,i,1,Delta) with 1/M
		if ((r0 == 1) && (r1 == 0)) {
			avalanche_probability[i] += 1/(float)nr_of_samples;
		}
		// If the difference is equal to 2, update p_(3,i,2,Delta) with 1/M
		else if ((r0 == 1) && (r1 == 1)) {
			avalanche_probability[192+i] += 1/(float)nr_of_samples;
		}
	}
}

/*
 * This function returns the avalanche entropy which is defined as:
 * H_{av} = sum(-p_(3,i,0,Delta)*log_q(p_(3,i,0,Delta)) - p_(3,i,1,Delta)*log_q(p_(3,i,1,Delta)) - p_(3,i,2,Delta)*log_q(p_(3,i,2,Delta)))
*/
float get_avalanche_entropy(float *avalanche_probability) {
	// Define a variable for the sum term
	float sum = 0;

	// Calculate the value for the sum term in the avalanche entropy
	for (int i = 0; i < 192; i++) {
		float value_p0 = (1 - avalanche_probability[i] - avalanche_probability[192+i]);
		sum += (- value_p0 * logarithm(value_p0, 3) 
			- avalanche_probability[i] * logarithm(avalanche_probability[i], 3)
			- avalanche_probability[192+i] * logarithm(avalanche_probability[192+i], 3));
	}

	// Return the value for the avalanche entropy
	return sum;
}

/*
 * This function returns the avalanche weight which is defined as:
 * overline(w)_{av} = sum(p_(3,i,1,Delta) + p_(3,i,2,Delta))
*/
float get_avalanche_weight(float *avalanche_probability) {
	// Define a variable for the sum term
	float sum = 0;

	// Calculate the value for the sum term in the avalanche weight
	for (int i = 0; i < 192; i++) {
		sum += (avalanche_probability[i] + avalanche_probability[192+i]);
	}

	// Return the value for the avalanche weight
	return sum;
}

/*
 * This function returns the avalanche dependence which is defined as:
 * D_{av} = n - (delta(p_(3,i,1,Delta)) + delta(p_(3,i,2,Delta))) with delta(x) = 1 if x = 0 and 0 otherwise
*/
int get_avalanche_dependence(float *avalanche_probability) {
	// Define a variable for the sum term
	int sum = 0;

	// Calculate the value for the sum term in the avalanche dependence
	for (int i = 0; i < 192; i++) {
		if ((avalanche_probability[i] == 0) && (avalanche_probability[192+i] == 0)) {
			sum += 1;
		}
	}

	// Return the value for the avalanche dependence
	return (192 - sum);
}

