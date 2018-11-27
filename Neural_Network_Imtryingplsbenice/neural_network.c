#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "setting_variables.h"

double sum_weights(double* input, double* weight, size_t len) 
// c'est la fonction de base pour caculer ce que ton neuronne doit calculer en gros,
// weight[0] c'est le bias, et le reste du tableau weight c'est les weights de chacun 
// de tes inputs.
{
	double sum = weight[0];
	for (size_t i = 1; i < len; ++i)
		sum += input[i -  1] * weight[i];
// input[i-1] car :
// tableau weights : [bias, w1, w2, w3, ..., wn]
// tableau d'input : [x1, x2, x3, ..., xn]
// tes weights lies au input commence a l'indice 1 tandis que les input a l'indice 0 --> tu as besoins de 
// ton input a l'indice [i-1]
	return sum;
}

//Computes the sigmoid of a value
double sigmoid(double val)
// c'est la fonction sigmoid tu vas pas me faire chier hein
{
	return 1 / (1 + exp(-val));
}

//Computes the sigmoid dash of a value, to use for emergency
double sigmoid_prime(double val)
{
	return sigmoid(val) * (1-(sigmoid(val)));
}

//Applies feedworward to a whole layer
void feedforward_layer(double* input, double** layer, size_t nbNeurones, size_t nbWeights)
{
	double* temp = malloc(sizeof(double) * nbNeurones);
	for (size_t i = 0; i < nbNeurones; ++i)
	// Tu fais ton iteration sur chacun des neuronnes de ta couche
		temp[i] = sigmoid(sum_weights(input, layer[i], nbWeights));
		// si t'as pas compris cette partie t'es debile et t'as mon numero
	input = temp;
	free(temp);
}


//Applies feedforward to the whole neural net
void feedforward(double* input, double*** network, size_t nblayer)
{
	for (size_t i = 0; i < nblayer; ++i)
		feedforward_layer(input, network[i], sizeof(network[i]) / sizeof(double*), sizeof(input)/sizeof(double));
}


/*double cost(double expected, double output)
{
	return pow((expected - output), 2);
}*/


//Computes the emergency to correct a node value (more or less a part of the gradient of its error)
double urgence(double expected, double output)
{
	return -1 * sigmoid_prime(output) * (output - expected);
}


//Computes the difference applied to a node bias
double diff_bias(double learning_rate, double urgence)
{
	return learning_rate * urgence;
}



//Computes the difference of a weight at a given stage
double diff_weight(double learning_rate, double urgence, double input)
{
	return learning_rate * urgence * input;
}

//Computes the difference applied to a node and its weights
void backprop_node(double* node, size_t len, double* input, double expected, double output)
{
	node[0] += diff_bias(0.3, urgence(expected, output));
	printf("%f ", node[0]);
	for (size_t i = 0; i < len; ++i)
		{
			node[i] += diff_weight(0.3, urgence(expected, output), input[i-1]);
			printf("%f ", node[i]);
		}
	printf("\n");
}

//Applies the backpropagation on all the nodes of a layer
void backprop_layer(double** layer,size_t len_node, size_t len, double* input, double* expected, double* output)
{
	for (size_t i = 0; i < len; i++)
		backprop_node(layer[i], len_node, input, expected[i], output[i]);
}

//Applies the backprop on the whole network
void backprop(double*** network, double* expected, double* output, double* input, size_t nblayers)
{
	for (size_t i = 0; i < nblayers; ++i)
		backprop_layer(network[i], sizeof(network[i][0])/sizeof(double), sizeof(network[i]) /sizeof(double*), input, expected, output);
}


//Iterates the feedforward then correct the neural network
void epoch(double*** network, double* input, double* expected)
{
	size_t nblayer = sizeof(network)/sizeof(double**);
	double* output = copy_array(input, sizeof(input) / sizeof(double));
	feedforward(output, network, nblayer);
	backprop(network, expected, output, input, nblayer);
	free(output);
}



//to replace by fonction
//Here instantiates matrices
int main()
{
	srand(time(NULL));
	double *input = malloc(sizeof(double) * 2);
	input[0] = (double) 1;
	input[1] = (double) 1;
	double ***network = (double***) (malloc(sizeof(double**) * 2));
	network[0] = (double**) (malloc((sizeof(double*) * 2)));
	network[1] = (double**) (malloc((sizeof(double*))));
	network[0][0] = (double*) (malloc(sizeof(double) * 3));
	network[0][1] = (double*) (malloc(sizeof(double) * 3));
	network[1][0] = (double*) (malloc((sizeof(double)) * 3));
	fill_array_random(network[0], 2, 3);
	fill_array_random(network[1], 1, 3);
	double* expected = malloc(sizeof(double));
	expected[0] = 0;
	for (size_t i = 0; i < 5000; ++i)
		epoch(network, input, expected);
	return 0;
}



