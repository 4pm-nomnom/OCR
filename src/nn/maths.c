#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


double sigmoid(double val)
{
    return 1.0/(1.0 + exp(-val));
}

double sum_weights(double* weights, double* input, size_t nbWeights)
{
    double sum = weights[0];
    for (size_t i = 1; i < nbWeights; ++i)
        sum += weights[i] * input[i-1];
    return sum;
}

double error_outputs(double target, double output)
{
    return 0.5 * pow((target - output), 2);
}

double total_error(double* outputs, double* inputs, size_t nbOutputs)
{
    double total_error = 0.0;
    for (size_t i = 0; i < nbOutputs; ++i)
        total_error += error_outputs(inputs[i], outputs[i]);
    return total_error;
}

double derrivative_Error_output(double target, double output)
{
    return output - target;
}

double derrivative_outpout(double output)
{
    return output * (1.0 - output);
}

double delta(double target, double output, double input)
{
    return derrivative_Error_output(target, output) *
        derrivative_outpout(output) * input;
}

double error_hidden_prev(double target, double output, double weight)
{
    return derrivative_Error_output(target, output) *
        derrivative_outpout(output) * weight;
}

double error_hidden(double target, double output, double output_n,
        double weight, double input)
{
    (void)input;
    return error_hidden_prev(target, output, weight) *
        derrivative_outpout(output_n); //* input;
}
