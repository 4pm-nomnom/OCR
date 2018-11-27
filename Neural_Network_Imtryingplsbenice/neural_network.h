#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK

double sun_weights(double* input, double* weight, size_t len);
double sigmoid(double val);
double sigmoid_prime(double val);
double* feedforward_layer(double* input, double** layer, size_t nbNeurones, size_t nbWeights);
double* feedforward(double *input, double*** network, size_t nblayers);
double urgence(double expected, double output);
double diff_bias(double expected, double output);
double diff_weight(double learning_rate, double urgence, double input);
void backprop_layer(double** layer, size_t len_node, size_t len, double* input, double* expected, double* output);
void epoch(double*** network, double* input, double* expected);

#endif
