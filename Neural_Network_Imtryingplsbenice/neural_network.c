#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "setting_variables.h"

#define LEARNING_RATE 0.05

//here we can but well have to replace it 
#define NB_WEIGHT 4 
#define NB_LAYER 3


// FEEDFORWARD
// Quite graspable


double sum_weights(double* input, double* weight)
// c'est la fonction de base pour caculer ce que ton neuronne doit calculer en gros,
// weight[0] c'est le bias, et le reste du tableau weight c'est les weights de chacun 
// de tes inputs.
{
	double sum = weight[0];
	for (size_t i = 1; i < NB_WEIGHT - 1; ++i)
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
	return -1 * sigmoid(val) * (1-(sigmoid(val)));
}


void feedforward_layer(double* input, double** layer, size_t nbNeurones)
{
	double* temp = malloc(sizeof(double) * nbNeurones);
	for (size_t i = 0; i < nbNeurones; ++i)
	// Tu fais ton iteration sur chacun des neuronnes de ta couche
		temp[i] = sigmoid(sum_weights(input, layer[i]));
		// si t'as pas compris cette partie t'es debile et t'as mon numero
	input = temp;
	free(temp);
}

void feedforward(double* input, double*** network, size_t nbLayers)
{
	for (size_t i = 0; i < nbLayers; ++i)
		feedforward_layer(input, network[i], nbneurones[i]);
}


/*double cost(double expected, double output)
{
	return pow((expected - output), 2);
}*/

//THE BACKPROPAGATION
//The backpropagation correct the target/"expected" value
//only modifies the last weight of a node, which is its value
//Give to each node an aim to consider with a given result.


//returns the value stored as last weight of a given node
double grab_value(double* node)
{
	return node[NB_WEIGHT - 1];

}

double sum_informations(double* input, double** nodelayer)
{
	double sum = 0;
	for (size_t i = 1; i < NB_WEIGHT-1; ++i)
	{
		double val = grab_value(nodelayer[i]);
		sum += val * input[i];
	}
	return sum;
}

//Computes the error margin for a node at the layer input of a given arc.
void error_information(double* input, double** nodelayer,size_t i)
{
	double sum = sum_informations(input, nodelayer);
	size_t value = NB_WEIGHT - 1;
	//node[i] the corresponding bias
	input[value] = sigmoid_prime(input[value]) * sum;
}



void backprop_layer(double **layer, double** input, size_t nbsynapse)
{
	for (size_t i = 1; i < nbsynapse; i++)
	{
		error_information(input, layer[i], i++);
	}
}


void backprop(double ***network, double nblayer)
{
	for (size_t i = 1; i < nblayer; ++i)
	{
		backprop_layer(network[i], network[i - 1], nbneurones[i]);
	}
}

double urgence(double expected, double output)
{
	return  -1 * sigmoid_prime(output) * (output - expected);
}

double diff_bias(double learning_rate, double urgence)
{
	return learning_rate * urgence;
}


double diff_weight(double learning_rate, double urgence, double input)
{
	return learning_rate * urgence * input;
}


// THE APPLIANCE
// Updates the weights and bias of the neuralnet with the new target/ value


void correct_node(double* node, double* input, double output)
{
	node[0] += diff_bias(LEARNING_RATE, urgence(node[NB_WEIGHT - 1], output));
	printf("bias = %f\n", node[0]);
	for (size_t i = 1; i < NB_WEIGHT - 1; ++i)
		{
			double var = diff_weight(LEARNING_RATE, urgence(node[NB_WEIGHT - 1], output), input[i-1]); 
			node[i] += var;
			printf("var = %f\nweight i = %f ", var, node[i]);
		}
	printf("\n");
}


void correct_layer(double** layer, double* input, double* output, size_t nbNeurones)
{
	for (size_t i = 0; i < nbNeurones; i++)
		correct_node(layer[i], input, output[i]);
}

void correct(double*** network, double* output, double* input, size_t nblayer)
{
	for (size_t i = 0; i < nblayer; ++i)
		correct_layer(network[i], input, output, nbneurones[i]);
}


void epoch(double*** network, double* input, double* expected)
{
	double* output = copy_array(input, 2);
	feedforward(output, network, NB_LAYER);
	backprop(network, NB_LAYER);
	correct(network, output, input, NB_LAYER);
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
	//bias + 2weights + expected value;
	network[0][0] = malloc(sizeof(double) * NB_WEIGHT);
	network[0][1] = malloc(sizeof(double) * NB_WEIGHT);
	network[1][0] = malloc(sizeof(double) * NB_WEIGHT);
	fill_array_random(network[0], 2, 3);
	fill_array_random(network[1], 1, 3);
	double* expected = malloc(sizeof(double));
	expected[0] = 0;
	for (size_t i = 0; i < 5000; ++i)
		epoch(network, input, expected);
	return 0;
}



