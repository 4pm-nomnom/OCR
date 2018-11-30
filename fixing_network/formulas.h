#ifndef FORMULAS_H
#define FORMULAS_H

double sigmoid(double val);

double sigmoid_prime(double val);

double grad(double targeted, double output);

double sum_weights(double* weights, double input, size_t nbWeights);

double sum_messages(double* errlayersucc, double** layer, size_t cst, size_t nbWeights);

double target(double input, double sum_weighted_messages);

void layer_cell_modif(double* errlayer, double* errlayersucc, double** layer, size_t nbNeurones, size_t nbWeights);

double error_margin(double returned, double expected);

double error_margin_grad(double returned, double expected);
