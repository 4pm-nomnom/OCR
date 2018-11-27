#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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


double sigmoid(double val)
// c'est la fonction sigmoid tu vas pas me faire chier hein
{
	return 1 / (1 + exp(-val));
}


double sigmoid_prime(double val)
{
	return sigmoid(val) * (1-(sigmoid(val)));
}


double* feedforward_layer(double* input, double** layer, size_t nbNeurones, size_t nbWeights)
{
	double* output = malloc (sizeof(double) * nbNeurones);
	// le tableau d'output passe dans la sigmoid de chacun de tes neuronnes
	double output_neuron;
	// bon ta race c'est juste le rendu de ta sigmoid a laquelle on a passe ta sum_weight
	for (size_t i = 0; i < nbNeurones; ++i)
	// Tu fais ton iteration sur chacun des neuronnes de ta couche
		output_neuron = sigmoid(sum_weights(input, layer[i], nbWeights));
		// si t'as pas compris cette partie t'es debile et t'as mon numero
	return output;
	// bah tu return ce que t'as trouve tu vas pas lacher ca dans la nature hein
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

void backprop_node(double* node, size_t len, double* input, double expected, double output)
{
	node[0] += diff_bias(0.3, urgence(expected, output));
	for (size_t i = 0; i < len; ++i)
		node[i] += diff_weight(0.3, urgence(exoected, output), input[i-1]);
}


void backprop_layer(double** layer,size_t len_node, size_t len, double* input, double* expected, double* output)
{
	for (size_t i = 0; i < len; i++)
		backprop_node(layer[i], len_node, input, expected[i], output[i]);
}


void modif_weights(double* neuron, double input*, double* expected, double* output)
{
	double urgence = 
	neuron[0] += urgence;
}





int main()
{
	srand(time(NULL));
	double *input = malloc(sizeof(double) * 2);
	input[0] = 1;
	input[1] = 0;
	double** layer_1 = malloc(sizeof(float*) * 2);
	layer_1[0] = malloc(sizeof(float) * 3);
	layer_1[1] = malloc(sizeof(float) * 3);
	fill_array_random(layer_1, 2, 3);
	double* output_1 = feedforward(input, layer_1, 2, 3);
	free(input);
	free(layer_1[0]);
	free(layer_1[1]);
	free(layer_1);
	printf("%f , %f\n", output_1[0], output_1[1]);
	free(output_1);
	free()
	return 0;
}



