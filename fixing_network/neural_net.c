#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "setting_variables.h"
#include "maths.h"
#include "formulas.h"


void feedforward_layer(double* input, double** layer, size_t nbNeurones, double* inputs)
{
	inputs = malloc(sizeof(double) * nbweights[0]);
	for (size_t i = 0; i < nbNeurones; ++i)
	{
	inputs[i] = sigmoid(sum_weights(input, layer[i], nbweights[i]));
	printf("Input[i] = %f\n",inputs[i]);
	}
}


void feedforward(double*** network, double** inputs, double* output, double** error, double* expected)
{
	size_t i;
	for(i = 0; i < nblayer - 1; ++i)
	{
		inputs[i+1] = malloc(sizeof(double*) * nbneurones[i]);
		feedforward_layer(inputs[i], network[i], nbneurones[i], inputs[i+1]);
	}
	feedforward_layer(inputs[i], network[i], nbneurones[i], output);
    for (size_t j = 0; j < nbneurones[i]; ++j)
    {
        error[i][j] = error_margin(expected[i] , output[j]);
        
    }
}



void correct(double*** network, double** error, double** inputs)
{
	for (size_t i = 0; i < nblayer; ++i)
		for (size_t j = 0; i < nbneurones[i]; ++i)
		{
			network[i][j][0] -= eta * error[i][j];
			for (size_t k = 1; k < nbweights[j]; ++k)
				{
					network[i][j][k] -= eta * error[i][j] * inputs[i][j];
					printf("new weight = %f\n", network[i][j][k]);
				}
		printf("\n");
		}
}


void backprop(double*** network, double** inputs, double** errors)
{
	int i = 1;
//	printf("coucou");
	for (; i >= 0; --i)
	{
		layer_cell_modif(errors[i - 1], errors[i], network[i - 1], nbneurones[i - 1], nbweights[i]);
	}
	for (size_t i = 0; i < nblayer; ++i)
		for (size_t j = 0; i < nbneurones[i]; ++i)
				printf("errors[%zu][%zu] = %f \n", i, j, errors[i][j]);
	for (size_t i = 0; i < nblayer; ++i)
		for (size_t j = 0; i < nbneurones[i]; ++i)
		{
			for (size_t k = 0; k < nbweights[j]; ++k)
			{	
			printf("network[%zu][%zu][%zu] = %f \n", i, j, k, network[i][j][0]);				
			}
		}
	correct(network, errors, inputs);
}

void epoch(double*** network, double* input, double** errors, double* expected)
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
	feedforward(network, inputs, output, errors, expected);
	//printf("Input[x] = %f\n", network[1][0][0]);
	backprop(network, inputs, errors);
	printf("Output = %f\n", output[0]);
}

void Train(double*** network, double **errors)
{
		for (size_t j = 0; j < 5000; ++j)
		{
				size_t i = rand() % 4;
				double* input = malloc(sizeof(double) * 2);
				input = _input[i];
				printf("\nInput 1 = %f \nInput 2 = %f\n", input[0], input[1]);
		double* expected = malloc(sizeof(double));
		expected[0] = (double) (i % 2);
				epoch(network, input, errors,expected);
//				input = _input[0];
		}
		size_t i = rand() % 4;
		double* input = malloc(sizeof(double) * 2);
		input = _input[i];
		printf("\nInput 1 = %f \nInput 2 = %f\n", input[0], input[1]);
		double* expected = malloc(sizeof(double));
		expected[0] = (double) (i % 2);
		epoch(network, input, errors, expected);
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
		double **errors;
		errors = malloc(2 * sizeof(double));
		errors[0] = malloc(2 *sizeof(double*));
		errors[1] = malloc(sizeof(double));
		fill_array_random(network[0], 2, 3);
		fill_array_random(network[1], 1, 3);
		Train(network, errors);
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


