#include "ternary.h"
#include "avalanche.h"


/*
 * This function tests whether the output is equal to the expected output and prints either a success or failure message
*/
void print_message(state s_test, state s_expected, char step[]) {
	printf("-------------------------------------------------------------------------------\n");
	for (int i = 0; i < 6; i++) {
		if ((s_test[i] ^ s_expected[i]) == 0) {
			printf("SUCCESS: the %s outputs the expected values for state[%d]\n", step, i);
		}
		else {
			printf("ERROR: the %s does not output the expected values for state[%d]\n", step, i);
		}
	}
	printf("-------------------------------------------------------------------------------\n\n");
}

/*
 * The main function: 
 * - Test round function (and thus step functions);
 * - Investigate avalanche behavior.
*/
int main() {
	/*// Define the states to use throughout the program
	state s = {
		0x0000000000000000, 
		0x0000000000000000,
		0x0000000000000000,
		0x0000000000000000,
		0x8000000000000000,
		0x0000000000000000
	};
	state s_test_theta_four = {
		0x0000000000000001,
		0x0000000000000000,
		0x0000000000000002,
		0x0000000000000000,
		0x8000000000000020,
		0x0000000000000000
	};
	state s_test_theta_three = {
		0x0000000000000000,
		0x0000000000000000,
		0x0000000000000410,
		0x0000000000000000,
		0x8000000000000024,
		0x0000000000000024
	};
	state s_test_rho = {
		0x0000000000000000,
		0x0000000000000000,
		0x0000000000208000,
		0x0000000000000000,
		0x8000000000000024,
		0x0000000000000024
	};
	state s_test_gamma = {
		0x0000000000001240,
		0x0000000000000000,
		0x0000000000208000,
		0x0000000000000000,
		0x8000000010400024,
		0x0000000000000024
	};

	// Run tests for round function
	printf("Perform theta step (four terms) and test it with PARAM_T = 6\n");
	theta_step_four_terms(s);
	print_message(s, s_test_theta_four, "theta step (four terms)");
	printf("Perform theta step (three terms) and test it with PARAM_TP = 5\n");
	theta_step_three_terms(s);
	print_message(s, s_test_theta_three, "theta step (three terms)");
	printf("Perform rho step and test it with PARAM_R0 = 0, PARAM_R1 = 11 and PARAM_R2 = 35\n");
	rho_step(s);
	print_message(s, s_test_rho, "rho step");
	printf("Perform gamma step and test it with PARAM_G = 7\n");
	gamma_step(s);
	print_message(s, s_test_gamma, "gamma step");*/
	
	// Avalanche metrics
	printf("Test the diffusion properties\n");
	printf("-------------------------------------------------------------------------------\n");
	float avalanche_probability[384];
	int nr_of_samples = 250000;
	int avalanche_dependence;
	float avalanche_weight;
	float avalanche_entropy;
	// These results can always obtain a max of 192, so 193 is a safe upper bound
	int min_dependence = 193;
	float min_weight = 193;
	float min_entropy = 193;
	int min_d = 0;
	int min_w = 0;
	int min_e = 0;

	for (int input_difference = 0; input_difference < 192; input_difference++) {
		for (int round = 0; round < 8; round++) {
			for (int i = 0; i < 384; i++) {
				avalanche_probability[i] = 0;	// reset the avalanche probability to 0
			}
			printf("Generate the avalanche probability vector\n");
			generate_avalanche_probability_vector(avalanche_probability, input_difference, nr_of_samples, round);
			avalanche_dependence = get_avalanche_dependence(avalanche_probability);
			if (min_dependence > avalanche_dependence) {
				min_dependence = avalanche_dependence;
				min_d = input_difference;
			}
			printf("Avalanche dependence with difference in bit %d after %d round(s): %d\n", input_difference, round, avalanche_dependence);
			avalanche_weight = get_avalanche_weight(avalanche_probability);
			if (min_weight > avalanche_weight) {
				min_weight = avalanche_weight;
				min_w = input_difference;
			}
			printf("Avalanche weight with difference in bit %d after %d round(s): %.1f\n", input_difference, round, avalanche_weight);
			avalanche_entropy = get_avalanche_entropy(avalanche_probability);
			if (min_entropy > avalanche_entropy) {
				min_entropy = avalanche_entropy;
				min_e = input_difference;
			}
			printf("Avalanche entropy with difference in bit %d after %d round(s): %.1f\n", input_difference, round, avalanche_entropy);
			printf("\n");
		}
	}
	printf("Mininum avalanche dependence on position %d: %d\n", min_d, min_dependence);
	printf("Minimum avalanche weight on position %d: %.1f\n", min_w, min_weight);
	printf("Minimum avalanche entropy on position %d: %.1f\n", min_e, min_entropy);
	printf("-------------------------------------------------------------------------------\n\n");

	// End of tests: return
	printf("main.c completed!\n");
	return 1;
}
