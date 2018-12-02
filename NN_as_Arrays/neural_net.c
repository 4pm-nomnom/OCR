#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "setting_variables.h"
#include "maths.h"


void feedforward_layer(double* input, double** layer, size_t nbNeurones, double* inputs)
{
	for (size_t i = 0; i < nbNeurones; ++i)
	{
	inputs[i] = sigmoid(sum_weights(input, layer[i], nbweights[i]));
//		printf("input = %f\n",inputs[i]);
	}
}


void feedforward(double*** network, double** inputs, double* output)
{
	size_t i;
	for(i = 0; i < nblayer - 1; ++i)
	{
		feedforward_layer(inputs[i], network[i], nbneurones[i], inputs[i+1]);
//		printf("inputs[%lu] = %f\n", i+1, inputs[i+1]);
	}
	feedforward_layer(inputs[i], network[i], nbneurones[i], output);
}


double backprop_node_out(double target, double output, double* inputs, size_t nbWeight)
{
	double error_out = 0;
	for (size_t i = 0; i < nbWeight; ++i)
	{
		error_out += delta(target, output, inputs[i]);
//		printf("error out = %f\n", error_out);
	}
	return error_out;
}


void backprop_layer_out(double* inputs, double* outputs, double* errors_out, double* targets, size_t nbNeurones)
{
	for (size_t i = 0; i < nbNeurones; ++i)
		errors_out[i] = backprop_node_out(targets[i], outputs[i], inputs, nbweights[i]);
}


double backprop_node(double* node, double* target, double* out, double output, double* inputs, size_t nbWeights)
{
	double error = 0;
	for (size_t i = 0; i < nbWeights; ++i)
		for (size_t j = 0; j < 2; ++j)
			{
		//		printf("backprop_node: \nTarget = %f, output = %f\n", target[0], output);
				error += error_hidden(target[i], out[j], output, node[i], inputs[j]);
//				printf("Error = %f\n",  error);
			}
//	printf("\n\n\n");
	return error; 
}


void backprop_layerH(double** layer, double* targets, double* output, double* out, double* in, double* errors, size_t nbNodes)
{
	for (size_t i = 0; i < nbNodes; ++i)
	{
		//	printf("backprop_layer: \ni is %ld    output is %f \n", i, out[i]);
		errors[i] = backprop_node(layer[i], targets, output, out[i], in, nbweights[i]);
//		printf("error[i] = %f\n", errors[i]);
	}
}

void correct(double*** network, double** error)
{
	for (size_t i = 0; i < 2; ++i)
		for (size_t j = 0; j < nbneurones[i]; ++j)
		{
			printf("error[%ld][%ld] = %f\n", i, j, error[i][j]);
			for (size_t k = 0; k < nbweights[i]; ++k)
				{
					network[i][j][k] -= eta * error[i][j];
					printf("new weight[%ld][%ld][%ld = %f\n",i,j,k, network[i][j][k]);
				}
	//	printf("\n");
		}
}


void backprop(double*** network, double* outputs, double* target, double** inputs)
{
	double** errors;
	errors = malloc(sizeof(double*) * nblayer);
	errors[0] = malloc(sizeof(double) * 2);
	errors[1] = malloc(sizeof(double));
	int i = 1;
//	printf("coucou");
	backprop_layer_out(inputs[1], outputs, errors[i], target, nbneurones[i]);
	--i;
	for (; i >= 0; --i)
	{
		backprop_layerH(network[i], target, outputs, inputs[i+1], inputs[i], errors[i], nbneurones[i]);
	}
	correct(network, errors);
}

void epoch(double*** network, double* input, double* expected)
{
	//printf("coucou");
	double **inputs;
	inputs = malloc(2 * sizeof(double*));
	inputs[0] = malloc(sizeof(double) * 2);
	inputs[0] = input;
	inputs[1] = malloc(sizeof(double) * 2);
	//printf("Input allocated");
	double* output;
	output = malloc(sizeof(double));
	feedforward(network, inputs, output);
	//printf("Input[x] = %f\n", input[1][0]);
	backprop(network, output, expected, inputs);
	printf("Output = %f\n", output[0]);
}

void Train(double*** network)
{
		for (size_t j = 0; j < 50; ++j)
		{
				size_t i = rand() % 4;
				double* input = malloc(sizeof(double) * 2);
				input = _input[i];
				printf("\nInput 1 = %f, Input 2 = %f\n", input[0], input[1]);
				double* expected = malloc(sizeof(double));
				expected[0] = (double) (i%2) ;
//				printf("Expected = %f\n", expected[0]);
				epoch(network, input, expected);
				input = _input[0];
		}
//		size_t i = rand() % 4;
//		double* input = malloc(sizeof(double) * 2);
//		input = _input[i];
//		printf("\nInput 1 = %f, Input 2 = %f\n", input[0], input[1]);
//		double* expected = malloc(sizeof(double));
//		expected[0] = (double) (i % 2);
//		epoch(network, input, expected);
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
