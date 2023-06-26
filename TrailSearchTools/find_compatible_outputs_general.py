import math

PARAM_G = 1

def write_to_file(f, offset, basis, input_difference):
	"""
	Write the offset P and basis V to the provided file.

		Parameters:
			f: file to which the offset P and basis V are printed
			offset: the offset P as given by P_i = b_i - (b_{i+1})^2
			basis: the basis V equal to b_i e_i if b_i is not equal to 0
			input_difference: the input difference of which all compatible outputs are requested
	"""
	# First, write the input difference to the file
	f.write('The compatible output differences of input difference ' + str(input_difference) + ' can be build using the following: \n\n')
	# Then, Write the offset to the file
	f.write('Offset P:\n' + str(offset) + '\n\n')
	# Lastly, write the basis to the file
	f.write('Basis vectors v_i in V: \n')
	for i in range(len(basis)):
		f.write(str(basis[i]) + '\n')


def find_compatible_output_differences(f, input_difference):
	"""
	Finds offset P and basis V of the compatible output differences belonging to the input difference b given.
	The offset is given by P_i = b_i - (b_{i+1})^2 and the basis is equal to b_i e_i if b_i is not equal to 0.
	Here, e_i is the standard basis: position i is equal to a single 1 and the rest is equal to 0.

		Parameters:
			f: file to which the offset P and basis V are printed
			input_difference: the input difference of which all compatible outputs are requested
	"""
	# Initialize an empty list V to hold basis vectors v_i and a list for offset P
	V = []
	P = [0] * len(input_difference)
	# For each state coordinate i, determine offset and basis
	for i in range(len(input_difference)):
		P[i] = (input_difference[i] - ((input_difference[(i+1) % len(input_difference)] ** 2) % 3)) % 3
		if input_difference[i] != 0:
			e = [0] * len(input_difference)
			e[i] = 1
			v = [elem * input_difference[i] for elem in e]
			V.append(v)
	# Write the results to the file data/compatible_outputs_general.txt
	write_to_file(f, P, V, input_difference)


if __name__=='__main__':
	# Define an input difference
	input_difference =  [2,0,1]

	# Check if valid PARAM_G is given
	if PARAM_G > len(input_difference):
		print("Parameter g is too large")
	if math.gcd(len(input_difference), PARAM_G) != 1:
		print("The length of the output difference and parameter g are not co-prime to each other")
	# If a valid PARAM_G is given
	else:
		# Open the file data/compatible_outputs_general.txt to save the offset P and basis V for the compatible output differences
		f = open('data/compatible_outputs_general.txt', 'w')

		# Find all compatible input differences
		find_compatible_output_differences(f, input_difference)

		# Finish the program with a line print and close the file
		print('Wrote offset P and basis V of the compatible outputs to data/compatible_outputs_general.txt (using parameter g = ' + str(PARAM_G) + ')')
		f.close()

