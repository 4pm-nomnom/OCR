#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "setting_variables.h"

double sum_weights(double* input, double* weight)
// c'est la fonction de base pour caculer ce que ton neuronne doit calculer en gros,
// weight[0] c'est le bias, et le reste du tableau weight c'est les weights de chacun 
// de tes inputs.
{
	double sum = weight[0];
	for (size_t i = 1; weight[i] != 0; ++i)
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
	return 1 / (1 + exp(-val));
}


double sigmoid_prime(double val)
{
	return sigmoid(val) * (1-(sigmoid(val)));
}


void feedforward_layer(double* input, double** layer)
{
	double* temp = malloc(sizeof(double) * nbNeurones);
	for (size_t i = 0; layer[i] != 0; ++i)
	// Tu fais ton iteration sur chacun des neuronnes de ta couche
		temp[i] = sigmoid(sum_weights(input, layer[i], nbWeights));
		// si t'as pas compris cette partie t'es debile et t'as mon numero
	input = temp;
	free(temp);
}

void feedforward(double* input, double*** network)
{
	for (size_t i = 0; network[i] != 0; ++i)
		feedforward_layer(input, network[i]);
}


/*double cost(double expected, double output)
{
	return pow((expected - output), 2);
}*/


double urgence(double expected, double output)
{
	return -1 * sigmoid_prime(output) * (output - expected);
}

double diff_bias(double learning_rate, double urgence)
{
	return learning_rate * urgence;
}


double diff_weight(double learning_rate, double urgence, double input)
{
	return learning_rate * urgence * input;
}

void backprop_node(double* node, double* input, double expected, double output)
{
	node[0] += diff_bias(0.3, urgence(expected, output));
	printf("bias = %f\n", node[0]);
	for (size_t i = 1; node[i] != 0; ++i)
		{
			node[i] += diff_weight(0.3, urgence(expected, output), input[i-1]);
			printf("weight i = %f ", node[i]);
		}
	printf("\n");
}


void backprop_layer(double** layer, double* input, double* expected, double* output)
{
	for (size_t i = 0; layer[i] != 0; i++)
		backprop_node(layer[i], input, expected[i], output[i]);
}

void backprop(double*** network, double* expected, double* output, double* input, size_t nblayer)
{
	for (size_t i = 0; network[i] < nblayer; ++i)
		backprop_layer(network[i], sizeof(network[i][0])/sizeof(double), sizeof(network[i]) /sizeof(double*), input, expected, output);
}


void epoch(double*** network, double* input, double* expected)
{
	size_t nblayer = 2;
	double* output = copy_array(input, sizeof(input) / sizeof(double));
	feedforward(output, network, 2);
	backprop(network, expected, output, input, 2);
	free(output);
}


int main()
{
	srand(time(NULL));
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
	return 0;
}



