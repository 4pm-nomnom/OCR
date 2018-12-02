#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "setting_variables.h"
#include "maths.h"

// computes each node output with respect to inputs
void feedforward_layer(double* input, double** layer, size_t nbNeurones, double* inputs)
{
	for (size_t i = 0; i < nbNeurones; ++i)
	{
	inputs[i] = sigmoid(sum_weights(input, layer[i], nbweights[i]));
	}
}

//gives the output with respect to a vector inputs
void feedforward(double*** network, double** inputs, double* output)
{
	size_t i;
	for(i = 0; i < nblayer - 1; ++i)
	{
		feedforward_layer(inputs[i], network[i], nbneurones[i], inputs[i+1]);
	}
	feedforward_layer(inputs[i], network[i], nbneurones[i], output);
}

//update a node error margin with respect to the cost of the feedforward output
double backprop_node_out(double target, double output, double* inputs, size_t nbWeight)
{
	double error_out = 0;
	for (size_t i = 0; i < nbWeight; ++i)
	{
		error_out += delta(target, output, inputs[i]);
	}
	return error_out;
}

// calls node error for the whole last layer
void backprop_layer_out(double* inputs, double* outputs, double* errors_out, double* targets, size_t nbNeurones)
{
	for (size_t i = 0; i < nbNeurones; ++i)
		errors_out[i] = backprop_node_out(targets[i], outputs[i], inputs, nbweights[i]);
}

//update a node error margin
double backprop_node(double* node, double* target, double* out, double output, double* inputs, size_t nbWeights)
{
	double error = 0;
	for (size_t i = 0; i < nbWeights; ++i)
		for (size_t j = 0; j < nbneurones[i - 1] ;++j)
			{
				error += error_hidden(target[0], output, out[i], node[j], inputs[j]);
			}
	return error; 
}

// Instantiate the errors of the whole layer
// in is the inputs of the layer in the network associated to the layer of the error matrix
void backprop_layerH(double** layer, double* targets, double* output, double* out, double* in, double* errors, size_t nbNodes)
{
	for (size_t i = 0; i < nbNodes - 1; ++i)
	{
		errors[i] = backprop_node(layer[i+1], targets, output, out[i], in, nbweights[i]);
	}
}


// Update each weight and bias according to its error margin
void correct(double*** network, double** error)
{
	for (size_t i = 0; i < 2; ++i)
		for (size_t j = 0; j < nbneurones[i]; ++j)
		{
			for (size_t k = 0; k < nbweights[i]; ++k)
				{
					network[i][j][k] -= eta * error[i][j];
				}
		}
}

//outputs is the vector given by the feedforward
// target is the value expected to reach
//inputs is  the vector having the inputs
// computes the error margin for each node, then update the weights according to its error margin
void backprop(double*** network, double* outputs, double* target, double** inputs)
{
	double** errors;
	errors = malloc(sizeof(double*) * nblayer);
	errors[0] = malloc(sizeof(double) * 2);
	errors[1] = malloc(sizeof(double));
	int i = 1;
	backprop_layer_out(inputs[1], outputs, errors[i], target, nbneurones[i]);
	--i;
	for (; i >= 0; --i)
	{
		backprop_layerH(network[i], target, outputs, inputs[i+1], inputs[i], errors[i], nbneurones[i]);
	}
	correct(network, errors);
}


//Calls the feedforward then the backpropagation 
// for given inputs
//knowing the expected result
void epoch(double*** network, double* input, double* expected)
{
	double **inputs;
	inputs = malloc(2 * sizeof(double*));
	inputs[0] = malloc(sizeof(double) * 2);
	inputs[0] = input;
	inputs[1] = malloc(sizeof(double) * 2);
	double* output;
	output = malloc(sizeof(double));
	feedforward(network, inputs, output);
	backprop(network, output, expected, inputs);
}


//Calls the feedforward then the backpropagation 
// for given inputs
//knowing the expected result
// Printing the output 
void displaying_epoch(double*** network, double* input, double* expected)
{
	double **inputs;
	inputs = malloc(2 * sizeof(double*));
	inputs[0] = malloc(sizeof(double) * 2);
	inputs[0] = input;
	inputs[1] = malloc(sizeof(double) * 2);
	double* output;
	output = malloc(sizeof(double));
	feedforward(network, inputs, output);
	backprop(network, output, expected, inputs);
	printf("Post_correction output = %f\n", output[0]);
}


//calls several time inputs
void Train(double*** network)
{
		for (size_t j = 0; j < 5000; ++j)
		{
				size_t i = rand() % 4;
				double* input = malloc(sizeof(double) * 2);
				input = _input[i];
				double* expected = malloc(sizeof(double));
				expected[0] = (double) (i%2) ;
				epoch(network, input, expected);
		}
}


//Pretty print of all nodes weights in the network
void print_network(double*** network, size_t i)
{
	for (size_t _i = 0; _i < i; ++_i)
	{
		printf("Weights in layer %lu :\n", _i);
		for (size_t _j = 0; _j < nbneurones[_i]; ++_j)
		{
			for (size_t _k = 0; _k < nbweights[_j]; _k++ )
				printf("| %f ", network[_i][_j][_k]);
			printf("|\n");
		}
	}
}


void Neural_network()
{
		srand(time(NULL));
		double ***network;
		network = malloc(2*sizeof(double**));
		network[0] = malloc(2*sizeof(double*));
		network[1] =  malloc(sizeof(double*));
		network[0][0] = malloc(sizeof(double) * 3);
		network[0][1] = malloc(sizeof(double) * 3);
		network[1][0] = malloc(sizeof(double) * 3);
		fill_array_random(network[0], 2, 3);
		fill_array_random(network[1], 1, 3);
		print_network(network, nblayer);
		Train(network);
		print_network(network, nblayer);
		double *input = malloc(sizeof(double) * 2);
		double* expected = malloc(sizeof(double));
		for (size_t i = 0; i < 4; ++i)
		{
			input = _input[i];
			printf("Input1 is %f	|| Input2 is %f\n", input[0], input[1]);
			expected[0] = (double) (i % 2);
			displaying_epoch(network, input, expected);
		}
		
}

int main()
{
	Neural_network();
	return 0;
}
