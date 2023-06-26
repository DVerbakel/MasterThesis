import math
import numpy as np
import itertools as it

PARAM_G = 1

def get_minimum_reverse_weight(output_difference):
	"""
	Finds the minimum reverse weight of the provided output difference:
	* First, the output difference is split in (wrapped) runs
	* If a run has an even length, output a value that is equal to half of this length
	* If a run has an odd length, output a value that is equal to half of this length plus 1
	* Sum over all these values: return the minimum reverse weight
	Code for the runs taken from https://stackoverflow.com/questions/19483909/finding-runs-within-a-list

		Parameters:
			output_difference: the output difference of which the minimum reverse weight is requested
	"""
	# Prepare the output difference for using the correct step size g
	output_difference_g = []
	position = 0
	for i in range(len(output_difference)):
		output_difference_g.append(output_difference[position])
		position = (position + PARAM_G) % len(output_difference)
		
	# Get the runs of an unwrapped array
	runs = [len(list(cgen)) for c,cgen in it.groupby(output_difference_g, lambda x: x>0) if c]
	
	# If no runs are found there are 0 runs present in the list
	if runs == []:
		runs.append(0)
	
	# Group the runs that wrap around the array
	if len(runs) > 1:
		if output_difference_g[0] >= 1 and output_difference_g[-1] >= 1:
			runs[0] += runs[-1]
			runs = runs[:-1]
	
	# Get the minimum reverse weight
	minimum_reverse_weight = 0
	for j in range(len(runs)):
		if runs[j] % 2 == 0: # If the length of a run is an even number
			minimum_reverse_weight += runs[j]/2
		else: # If the length of a run is an odd number
			minimum_reverse_weight += int(runs[j]/2)+1 
	
	return int(minimum_reverse_weight)


if __name__=='__main__':
	# Define the output difference of which we want to know the minimum reverse weight
	output_difference = [0,1,0,1,0]

	# Check if valid PARAM_G is given
	if PARAM_G > len(output_difference):
		print("Parameter g is too large")
	if math.gcd(len(output_difference), PARAM_G) != 1:
		print("The length of the output difference and parameter g are not co-prime to each other")
	# If a valid PARAM_G is given
	else:
		# Find the minimum reverse weight
		minimum_reverse_weight = get_minimum_reverse_weight(output_difference)

		# Finish the program with a line print
		print('Minimum reverse weight: ' + str(minimum_reverse_weight) + ' (using parameter g = ' + str(PARAM_G) + ')')

