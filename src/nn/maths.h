#ifndef MATH_H
#define MATH_H

double sigmoid(double val);

double sum_weights(double* weights, double* input, size_t nbWeights);

double error_outputs(double target, double output);

double total_error(double* outputs, double* inputs, size_t nbOtputs);

double derrivative_Error_output(double target, double output);

double derrivative_outpout(double output);

double delta(double target, double input, double output);

double error_hidden_prev(double target, double output);

double error_hidden(double target, double output, double output_n,
        double weight, double input);

#endif
