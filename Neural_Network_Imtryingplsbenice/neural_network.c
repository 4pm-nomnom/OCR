#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "setting_variables.h"

double sum_weights(double* input, double* weight, size_t nbWeight)
// c'est la fonction de base pour caculer ce que ton neuronne doit calculer en gros,
// weight[0] c'est le bias, et le reste du tableau weight c'est les weights de chacun 
// de tes inputs.
{
	double sum = weight[0];
	for (size_t i = 1; i < nbWeight; ++i)
		sum += input[i -  1] * weight[i];
// input[i-1] car :
// tableau weights : [bias, w1, w2, w3, ..., wn]
// tableau d'input : [x1, x2, x3, ..., xn]
// tes weights lies au input commence a l'indice 1 tandis que les input a l'indice 0 --> tu as besoins de 
// ton input a l'indice [i-1]
	return sum;
}


double sigmoid(double val)
// c'est la fonction sigmoid tu vas pas me faire chier hein
{
	double sigmoid = 1.0 / (1.0 + exp(-val));
	return sigmoid;
}


double sigmoid_prime(double val)
{
	return sigmoid(val) * (1.0-(sigmoid(val)));
}


double* feedforward_layer(double* input, double** layer, size_t nbNeurones)
{
	double* temp = malloc(sizeof(double) * nbNeurones);
	for (size_t i = 0; i < nbNeurones; ++i)
	// Tu fais ton iteration sur chacun des neuronnes de ta couche
		temp[i] = sigmoid(sum_weights(input, layer[i], nbweights[i]));
		// si t'as pas compris cette partie t'es debile et t'as mon numero
	return temp;
}

double* feedforward(double* input, double*** network, size_t nbLayers)
{
	for (size_t i = 0; i < nbLayers; ++i)
	{
		input = feedforward_layer(input, network[i], nbneurones[i]);
	}
	return input;
}


/*double cost(double expected, double output)
{
	return pow((expected - output), 2);
}*/


double urgence(double expected, double output)
{
	double urgence = -1.0 * sigmoid_prime(output) * (output - expected);
	//printf("Output = %f\n", output);
	return urgence;
}

double diff_bias(double learning_rate, double urgence)
{
	return learning_rate * urgence;
}


double diff_weight(double learning_rate, double urgence, double input)
{
	return learning_rate * urgence * input;
}

void backprop_node(double* node, double* input, double expected, double output, size_t nbWeight)
{
	node[0] += diff_bias(0.03, urgence(expected, output));
	//printf("bias = %f\n", node[0]);
	for (size_t i = 1; i < nbWeight; ++i)
		{
			node[i] += diff_weight(0.03, urgence(expected, output), input[i-1]);
	//		printf("weight i = %f ", node[i]);
		}
}


void backprop_layer(double** layer, double* input, double* expected, double* output, size_t nbNeurones)
{
	for (size_t i = 0; i < nbNeurones; i++)
	{
		backprop_node(layer[i], input, expected[i], output[i], nbweights[i]);
	}
}

void backprop(double*** network, double* expected, double* output, double* input, size_t nblayer)
{
	for (size_t i = 0; i < nblayer; ++i)
		backprop_layer(network[i], input, expected, output, nbneurones[i]);
}


void epoch(double*** network, double* input, double* expected)
{
	double* output = malloc(2 * sizeof(double));
	copy_array(input, 2, output);
	output = feedforward(output, network, nblayer);
	backprop(network, expected, output, input, nblayer);
	free(output);
}


int main()
{
	double *input = malloc(sizeof(double) * 2);
	input[0] = (double) 1;
	input[1] = (double) 1;
	double ***network;
	network = malloc(2*sizeof(double**));
	network[0] = malloc(2*sizeof(double*));
	network[1] =  malloc(sizeof(double*));
	network[0][0] = malloc(sizeof(double) * 3);
	network[0][1] = malloc(sizeof(double) * 3);
	network[1][0] = malloc(sizeof(double) * 3);
	fill_array_random(network[0], 2, 3);
	fill_array_random(network[1], 1, 3);
	double* expected = malloc(sizeof(double));
	expected[0] = 0;
	for (size_t i = 0; i < 5000; ++i)
		epoch(network, input, expected);
	input[0] = 0;
	input[1] = 1;
	expected[0] = 1;
	epoch(network, input, expected);
	return 0;
}



