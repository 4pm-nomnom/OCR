#include <stdlib.h>
#include <math.h>
#include <time.h>

double sigmoid(double val)
{
	return 1.0/(1.0 + exp(-val));
}

double sigmoid_prime(double val)
{
	double sig = sigmoid(val);
	return sig * (1.0 - sig);
}

double grad(double targeted, double output)
{
	return 2* (output - target);
}


// weights are from layer[i]
// input are from layer[i - 1]
// computes the val given to the sigmoid for a node of the layer[i]
double sum_weights(double *weights, double* input, size_t nbWeights)
{
	// sum of node = its bias;
	double sum = weights[0];
	for (size_t i =1; i < nbWeights; ++i)
		// explicit
		sum +=weights[i] * input[i - 1];
	return sum;
}

//errlayersucc is the layer[i + 1] of the error network
//node is a node of the layer[i] OF THE NN
double sum_messages(double* errlayersucc, double** layer, size_t cst, size_t nbWeights)
{
	double sum = 0;
	for (size_t k = 1; k < nbWeights; ++k)
		sum += errlayersucc[i - 1] * layer[i][cst];
	return sum;
}

// input is the node of layer[i] whose target is replaced
// sum_weighted_messages is the val of biased targets from the layer[i + 1]
double target(double input, double sum_weighted_messages)
{
	return sigmoid_prime(input) * sum_weighted_messages;
}


//errlayer is the layer[i] of the error network
// of nbNeurones synapses
//errlayersucc is the layer[i + 1] of the error network
// of nbweights synapses
//as layer
//modifies the whole layer
void cell_modif(double* errlayer, double* errlayersucc, double** layer,size_t nbNeurones, size_t nbWeights)
{
	double sum;
	for (size_t i = 0; i < nbNeurones; ++i)
	{
		sum = sum_messages(errlayersucc, layer, i, nbWeights);
		errlayer[i] = target(node[i], sum);
	}
}


//modifies each layer
void backpropagation

