#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "setting_variables.h"
#include "maths.h"


void feedforward_layer(double* input, double** layer, size_t nbNeurones, double* inputs)
{
	inputs = malloc(sizeof(double) * nbweights[0]);
	for (size_t i = 0; i < nbNeurones; ++i)
		inputs[i] = sigmoid(sum_weights(input, layer[i], nbweights[i]));
}


void feedforward(double*** network, double** inputs, double* output)
{
	size_t i;
	for(i = 0; i < nblayer - 1; ++i)
	{
		inputs[i+1] = malloc(sizeof(double*) * nbneurones[i]);
		feedforward_layer(inputs[i], network[i], nbneurones[i], inputs[i+1]);
	}
	feedforward_layer(inputs[i], network[i], nbneurones[i], output);
}


void backprop_node_out(double target, double output, double* inputs, double* error_out, size_t nbWeight)
{
	for (size_t i = 0; i < nbWeight; ++i)
		error_out[i] = delta(target, output, inputs[i]);
}


void backprop_layer_out(double* inputs, double* outputs, double** errors_out, double* targets, size_t nbNeurones)
{
	for (size_t i = 0; i < nbNeurones; ++i)
		backprop_node_out(targets[i], outputs[i], inputs, errors_out[i], nbweights[i]);
}


void backprop_node(double* node, double* target, double* out, double output, double* inputs, double* errors, size_t nbWeights)
{
	for (size_t i = 1; i < nbWeights; ++i)
		for (size_t j = 0; j < nbinputs; ++j)
			errors[i] += error_hidden(target[j], out[j], output, node[i], inputs[i - 1]);
}


void backprop_layerH(double** layer, double* targets, double* output, double* out, double* in, double** errors, size_t nbNodes)
{
	for (size_t i = 0; i < nbNodes; ++i)
	{
		errors[i] = malloc(sizeof(double) * nbweights[i]);
		backprop_node(layer[i], targets, output, out[i], in, errors[i], nbweights[i]);
	}
}

void correct(double*** network, double*** error)
{
	for (size_t i = 0; i < nblayer; ++i)
		for (size_t j = 0; i < nbneurones[i]; ++i)
		{
			network[i][j][0] -= eta * error[i][j][0];
			for (size_t k = 1; k < nbweights[j]; ++k)
				network[i][j][k] -= eta * error[i][j][k];
		}
}


void backprop(double*** network, double* outputs, double* target, double** inputs)
{
	double*** errors = malloc(sizeof(double**) * nblayer);
	int i = 1;
	backprop_layer_out(inputs[i], outputs, errors[i], target, nbneurones[i]);
	for (; i >= 0; --i)
	{
		errors[i] = malloc(sizeof(double*) * nbneurones[i]);
		backprop_layerH(network[i], target, outputs, inputs[i+1], inputs[i-1], errors[i], nbneurones[i]);
	}
	correct(network, errors);
}

void epoch(double*** network, double* input, double* expected)
{
	double** inputs = malloc(nblayer * sizeof(double*));
	inputs[0] = input; 
	double* output = malloc(sizeof(double) * nbneurones[nblayer-1]);
	feedforward(network, inputs, output);
	backprop(network, output, expected, inputs);
}

void Train(double*** network)
{
		for (size_t j = 0; j < 20000; ++j)
		{
				size_t i = rand() % 4;
				double* input = malloc(sizeof(double) * 2);
				input = _input[i];
				printf("\nInput 1 = %f, Input 2 = %f\n", input[0], input[1]);
				double* expected = malloc(sizeof(double));
				expected[0] = (double) (i%2) ;
				printf("Expected = %f\n", expected[0]);
				epoch(network, input, expected);
				input = _input[0];
		}
		size_t i = rand() % 4;
		double* input = malloc(sizeof(double) * 2);
		input = _input[i];
		printf("\nInput 1 = %f, Input 2 = %f\n", input[0], input[1]);
		double* expected = malloc(sizeof(double));
		expected[0] = (double) (i % 2);
		epoch(network, input, expected);
}


int main()
{
		//      double *input = malloc(sizeof(double) * 2);
		//      input[0] = (double) 1;
		//      input[1] = (double) 1;
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
		Train(network);
		//      double* expected = malloc(sizeof(double));
		//      expected[0] = 0;
		//for (size_t i = 0; i < 5000; ++i)
		//      epoch(network, input, expected);
		//input[0] = 0;
		//input[1] = 1;
		//expected[0] = 1;
		//epoch(network, input, expected);
		return 0;

}
