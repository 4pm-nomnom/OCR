#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "setting_variables.h"
#include "formulas.h"


void feedforward_layer(double* input, double** layer, size_t nbNeurones, double* inputs)
{
	for (size_t i = 0; i < nbNeurones; ++i)
	{
	inputs[i] = sigmoid(sum_weights(input, layer[i], nbweights[i]));
	//printf("in layer inputs[%zu] is %f \n", i, inputs[i]);
	}
}


void feedforward(double*** network, double** inputs, double* output, double** error, double* expected)
{
	size_t i;
	for(i = 0; i < nblayer - 1; ++i)
	{
		feedforward_layer(inputs[i], network[i], nbneurones[i], inputs[i+1]);
	//	printf("out inputs[%zu] is %f\n", i + 1, inputs[i + 1][i]);
	}
	feedforward_layer(inputs[i], network[i], nbneurones[i], output);
	//printf("layer pre out[0] = %f\n", output[0]);
		for (size_t j = 0; j < nbneurones[i]; ++j)
    	{
    		error[i][j] = error_margin(expected[i] , output[j]);
//			printf("error[%ld],[%ld] = %f\n",i, j, error[i][j] );
  		}
//	printf("output[0] = %f\n", output[0]);
}



void correct(double*** network, double** error, double** inputs)
{
		size_t i;
		//printf("Errors[0][0] is %f\ninputs[0][0] is %f\n", error[0][0], inputs[0][0]);
	for (i = 0; i < nblayer; ++i)
		for (size_t j = 0; j < nbneurones[i]; ++j)
		{
			//printf("Bias of the [%ld] in the layer[%ld] is %f \n", j, i, network[i][j][0]);
			network[i][j][0] -= eta * error[i][j];
			//printf("Bias of the [%ld] in the layer[%ld] is %f \n", j, i, network[i][j][0]);
			for (size_t k = 1; k < nbweights[j]; ++k)
				{
				//printf("Weight[%ld]  of the [%ld] in the layer[%ld] is %f \n", k, j, i, network[i][j][k]);
				printf("inputs[%ld][%ld] is %f\n", i, j, inputs[i][j]);
					network[i][j][k] -= eta * error[i][j] * inputs[i][j];
				//printf("Weight[%ld]  of the [%ld] in the layer[%ld] is %f \n", k, j, i, network[i][j][k]);
				}
		}
}


void backprop(double*** network, double** inputs, double** errors)
{
	int i = 1;
	for (;i>0;--i)
	{
		layer_cell_modif(errors[i - 1],errors[i],network[i - 1],nbneurones[i - 1 ],nbweights[i]);
//        printf("errors[%u][0] is %f \nerrors[%u][1] is %f \n",i - 1,errors[i-1][0],i - 1,errors[i][1]);
//        printf("errors[%u][0] is %f \n",i ,errors[i][0]);
//        printf("nbneurones[%u] is %ld \nweight[%u] is %ld", i - 1 , nbneurones[i - 1], i, nbweights[i]);
	}
//	for (size_t i = 0; i < nblayer; ++i)
		//for (size_t j = 0; i < nbneurones[i]; ++i)
				//printf("errors[%zu][%zu] = %f \n", i, j, errors[i][j]);
//	for (size_t i = 0; i < nblayer; ++i)
//		for (size_t j = 0; i < nbneurones[i]; ++i)
//		{
//			for (size_t k = 0; k < nbweights[j]; ++k)
//			{	
			//printf("network[%zu][%zu][%zu] = %f \n", i, j, k, network[i][j][0]);				
//			}
//		}
	correct(network, errors, inputs);
}

void epoch(double*** network, double* input, double* expected)
{
	double** errors;
	errors = malloc(2 * sizeof(double));
	errors[0] = malloc(2 *sizeof(double*));
	errors[1] = malloc(sizeof(double));
	double **inputs;
	inputs = malloc(2 * sizeof(double*));
	inputs[0] = malloc(sizeof(double) * 2);
	inputs[0] = input;
	inputs[1] = malloc(sizeof(double) * 2);
	double* output;
	output = malloc(sizeof(double));
	feedforward(network, inputs, output, errors, expected);
	backprop(network, inputs, errors);
	printf("Output = %f\n \n \n", output[0]);
}

void Train(double*** network)
{
		for (size_t j = 0; j < 1; ++j)
		{
			size_t i = rand() % 4;
			double* input = malloc(sizeof(double) * 2);
			input = _input[i];
//			printf("\nInput 1 = %f \nInput 2 = %f\n", input[0], input[1]);
			double* expected = malloc(sizeof(double));
			expected[0] = (double) (i % 2);
			epoch(network, input,expected);
//			input = _input[0];
		}
		size_t i = rand() % 4;
		double* input = malloc(sizeof(double) * 2);
		input = _input[i];
		printf("\nInput 1 = %f \nInput 2 = %f\n", input[0], input[1]);
		double* expected = malloc(sizeof(double));
		expected[0] = (double) (i % 2);
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
