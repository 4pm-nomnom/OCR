#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "setting_variables.h"
#include "maths.h"

void feedforward_layer(double* input, double** layer, size_t nbNeurones, double* inputs)
{
	for (size_t i = 0; i < nbNeuronnes; ++i)
		inputs[i] = sigmoid(sum_weights(input, layer[i], nbweights[i]));
}

void feedforward(double*** network, double** inputs, double* output)
{
	size_t i;
	for(i = 0; i < nbLayers - 1; ++i)
		feedforward_layer(inputs[i], network[i], nbNeurones[i], inputs[i+1]);
	feedforward_layer(inputs[i], network[i], nbNeurones[i], outputs);
}

void backprop_node_out(double target, double output, double* inputs, double* error_out, size_t nbWeight)
{
	for (size_t i = 0; i < nbWeigth; ++i)
		error_out[i] = delta(target, output, inputs[i]);
}

void backprop_layer_out(double* inputs, double* outputs, double* errors_out, double* targets, size_t nbNeurones)
{
	for (size_t i = 0; i < nbNeurones, ++i)
		errors_out[i] = backprop_node_out(targets[i], outputs[i], inputs, errors_out, nbweight[i]);
}

void backprop_node(double* node, double* target, double* out, double output, double* inputs, double* errors, size_t nbWeights)
{
	for (size_t i = 1; i < nbWeights; ++i)
		for (size_t j = 0; j < nbout; ++j)
			errors[i] += error_hidden(target[j], out[j], output, node[i], inputs[i - 1]);
}

void backprop_layerH(double** layer, double* targets, double* output, double* out, double* in, double** errors, size_t nbNodes)
{
	for (size_t i = 0; i < nbNodes; ++i)
	{
		errors[i] = malloc(sizeof(double) * nbWeights[i]);
		backprop_node(layer[i], targets, output, out[i], in, errors[i], nbWeights[i]);
	}
}

void backprop(double***i network, double* outputs, double* target, double** inputs)
{
	double*** errors = malloc(sizeof(double**) * nbLayers);
	errors[0] = backprop_layer_out(, target, outputs, inputs[nbLayers-1], inputs)
	for (size_t i = 0; i < nbLayers, ++i)
	{
		errors[i] = malloc(sizeof(double*) * nbNodes[i]);
		backprop_layerH()



