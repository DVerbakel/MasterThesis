import math

PARAM_G = 1

def write_to_file(f, input_general, index, output_difference):
	"""
	Replace all non-zero x values to a 1 and/or 2 depending on the location of a zero value:
	* If there exists a pattern x 0, this x is fixed to the value as in output difference
	* If there exists a pattern x x, the first x can both be a 1 or a 2
	Code adapted from https://www.geeksforgeeks.org/generate-all-binary-strings-from-given-pattern/ 

		Parameters:
			f: file to which compatible (output difference,input difference)-pairs are printed
			input_general: the input difference using the non-zero and zero placeholders x and 0
			index: counter for the position in a certain input and output difference
			output_difference: the output difference of which all compatible inputs are requested
	"""
	# If we iterated through all positions, write the compatible input difference to the file
	if index == len(input_general):
		f.write(str(output_difference) + ' - ' + str([int(x) for x in input_general]) + '\n')
		return

	# If there exists a pattern x 0, fix the x value by the value as in output difference
	if input_general[index] == 'x' and input_general[(index+PARAM_G)%len(input_general)] == '0':
		input_general[index] = str(output_difference[index])
		write_to_file(f, input_general, index+1, output_difference)
		input_general[index] = 'x'
	# Else if there exists the pattern x x, set the first x to a 1 and a 2
	elif input_general[index] == 'x':
		input_general[index] = '1'
		write_to_file(f, input_general, index+1, output_difference)
		input_general[index] = '2'
		write_to_file(f, input_general, index+1, output_difference)
		input_general[index] = 'x'
	# Else (if we encounter a zero value), continue to the next position
	else:
		write_to_file(f, input_general, index+1, output_difference)

def valid_child(child_value, output_difference_general, i):
	"""
	Checks if a child has a valid value. A child value should not have the following:
	* The bound of non-zero values cannot be higher than 20
	* If the output difference is 0, the input difference cannot be x 0
	* If the output difference is x, the input difference cannot be 0 0

		Parameters:
			child_value: the possible compatible input difference
			output_difference_general: the output difference of which all compatible inputs are requested in string notation
			i: the position on which a 0 or x is added
	"""
	# If the child value exceeds the bound, the child is invalid
	if child_value.count('x') > 20:
		return False

	# If the child value violates one of the two impossible rules, the child is invalid
	if output_difference_general[i] == '0' and child_value[i] == 'x' and child_value[(i+PARAM_G) % len(output_difference_general)] == '0':
		return False
	if output_difference_general[i] == 'x' and child_value[i] == '0' and child_value[(i+PARAM_G) % len(output_difference_general)] == '0':
		return False
	# Check if the wrap around value is still valid
	if i == (len(output_difference_general)-1+PARAM_G) % len(output_difference_general):
		if output_difference_general[(i-PARAM_G) % len(output_difference_general)] == '0' and child_value[(i-PARAM_G) % len(output_difference_general)] == 'x' and child_value[i] == '0':
			return False
		if output_difference_general[(i-PARAM_G) % len(output_difference_general)] == 'x' and child_value[(i-PARAM_G) % len(output_difference_general)] == '0' and child_value[i] == '0':
			return False

	# If the child does not trigger one of the two above, the child is valid
	return True


def find_children(f, queue, output_difference_general, output_difference):
	"""
	Finds the left and right child of the first element in the queue.
	Possibly adds the left and/or right child to the queue if the value is valid.

		Parameters:
			f: file to which compatible (output difference,input difference)-pairs are printed
			queue: list of children to be visited
			output_difference_general: the output difference of which all compatible inputs are requested in string notation
			output_difference: the output difference of which all compatible inputs are requested
	"""
	if queue[0][1] >= 0:
		# Set the values of the children
		left_child = queue[0][0].copy()
		left_child[queue[0][1]] = 'x'
		right_child = queue[0][0].copy()
		right_child[queue[0][1]] = '0'
		
		# Find the position of the next unknown value to change. 
		# If value equals to original value: possible compatible input is found by filling in unknown value
		pos_unknown = (queue[0][1]-PARAM_G)%len(output_difference)
		if pos_unknown == len(output_difference)-1:
			pos_unknown = -1

		# Check if the child value is still valid. If not: prune tree
		if valid_child(right_child, output_difference_general, queue[0][1]):
			queue.insert(1, (right_child, pos_unknown))
		if valid_child(left_child, output_difference_general, queue[0][1]):
			queue.insert(1, (left_child, pos_unknown))
	
	else:
		# Write the compatible input to the file
		write_to_file(f, queue[0][0], 0, output_difference)


def find_compatible_input_differences(f, output_difference):
	"""
	Finds all compatible input differences belonging to the output difference given.
	This is done through a left-sided depth-first search in a tree.

		Parameters:
			f: file to which compatible (output difference,input difference)-pairs are printed
			output_difference: the output difference of which all compatible inputs are requested
	"""
	# Initialize the root as all unknown values and start the queue with this root
	root = ['_'] * len(output_difference)
	queue = [(root,len(output_difference)-1)]

	# Transform the output difference to zero values '0' and non-zero values 'x'
	output_difference_general = [str(x) for x in output_difference]
	output_difference_general = list(map(lambda x: x.replace('1','x'), output_difference_general))
	output_difference_general = list(map(lambda x: x.replace('2','x'), output_difference_general))	

	# While the queue is not empty, traverse the tree (DFS, left) with its children
	while queue:
		find_children(f, queue, output_difference_general, output_difference)
		queue.pop(0)


if __name__=='__main__':
	# Define an output difference
	output_difference =  [1,1,1]

	# Check if valid PARAM_G is given
	if PARAM_G > len(output_difference):
		print("Parameter g is too large")
	if math.gcd(len(output_difference), PARAM_G) != 1:
		print("The length of the output difference and parameter g are not co-prime to each other")
	# If a valid PARAM_G is given
	else:
		# Open the file data/compatible_inputs_general.txt to save the compatible input differences of the provided output difference
		f = open('data/compatible_inputs_general.txt', 'w')

		# Find all compatible input differences
		find_compatible_input_differences(f, output_difference)

		# Finish the program with a line print and close the file
		print('Wrote compatible inputs to data/compatible_inputs_general.txt (using parameter g = ' + str(PARAM_G) + ')')
		f.close()

