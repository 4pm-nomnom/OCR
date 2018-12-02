#include "xorgate.h"
#include "nodexor.h"
#include "xorfun.h"
#include <stdio.h>
#include <math.h>


//If the NeuralNetwork is initialized with proper values,
// Behaves like a XOR gate
//Else, behaves like a OR gate

//compute the sum of product with the bias
float ff(HiddenLayer H, OutputLayer O)
{
    float w1 = H.Node1.weight;
    float x1 = H.Node1.value;
    float w2 = H.Node2.weight;
    float x2 = H.Node2.value;
    float bias = O.bias;
    float output =  (-1.0f) * (w1 * x1 + w2 * x2 + bias);
    float sigmoid = 1/ (1 + exp(output));
    return sigmoid;
}


//compute the sum of product with the bias for the node1
float ffdnode1(InputLayer I, HiddenLayer H,float x1, float x2)
{

    float w1 = I.Node1.weight1;
    float w2 = I.Node2.weight1;
    float bias = H.Node1.bias;
    float output =  (-1.0f) * (x1* w1 + x2 * w2 + bias);
    float sigmoid = 1/ (1 + exp(output));
    return sigmoid;
}


//compute the sum of product with the bias for the node2
float ffdnode2(InputLayer I, HiddenLayer H,float x1, float x2)
{

    float w1 = I.Node1.weight2;
    float w2 = I.Node2.weight2;
    float bias = H.Node2.bias;
    float output =  (-1.0f) * (x1* w1 + x2 * w2 + bias);
    float sigmoid = 1/ (1 + exp(output));
    return sigmoid;
}


float feedforward(NeuralXorNet A, float x1, float x2)
{

    A.HiddenLayer.Node1.value = ffdnode1(A.InputLayer, A.HiddenLayer, x1, x2);
    A.HiddenLayer.Node2.value = ffdnode2(A.InputLayer, A.HiddenLayer, x1, x2);
    A.OutputLayer.res = ff(A.HiddenLayer, A.OutputLayer);
    return A.OutputLayer.res;
}

//a floating square
float fsquare(float x1)
{
    return x1*x1;
}


// estimates between the output and the expected result.
float costout(NeuralXorNet A, float x1, float x2)
{
    //the awaited value for xor of the inputs
    float xor = xorfun(x1, x2);
    // the value we add
    float res = feedforward(A, x1, x2);
    // we want to modify the weight and bias to be closer from the xor values
    float cost = 0.5f * fsquare(xor-res);
    if (xor == 0.0f)
        cost = (-1.0f) * cost;
    return cost;
}


// estimates between the output and the expected result.
float costhidden1(NeuralXorNet A, float x1, float x2)
{
    //the awaited value for xor of the inputs
    float and = speand1(x1, x2);
    // the value we have
    float res = ffdnode1(A.InputLayer, A.HiddenLayer, x1, x2);
    // we want to modify the weight and bias to be closer from the value
    float cost = 0.5f * fsquare(and-res);
    if (and == 0.0f)
        cost = (-1.0f) * cost;
    return cost;
}


// estimates between the output and the expected result.
float costhidden2(NeuralXorNet A, float x1, float x2)
{
    //the awaited value for xor of the inputs
    float and = speand2(x1, x2);
    // the value we have
    float res = ffdnode2(A.InputLayer, A.HiddenLayer, x1, x2);
    // we want to modify the weight and bias to be closer from the value
    float cost = 0.5f * fsquare(and-res);
    if (and == 0.0f)
        cost = (-1.0f) * cost;
    return cost;
}


float operrorh1(InputLayer I, HiddenLayer H, float x1, float x2, int coord)
{
    // The output using the neuralnet, a
    float a = 0;
    // The wanted one, y(x)
    float yx = speand1(x1,x2);
    //We need the derivative of sigmoid of inputs
    //variable declaration	float w1 = NodeXor.weight;
    float w1 = I.Node1.weight1;
    float w2 = I.Node2.weight1;
    float bias = H.Node1.bias;
    //Result of partial der of cost here
    float output = (-1.0f) * (x1 * w1 + x2 * w2 + bias);
    float sigmoid = 1/(1 + exp(output));
    //sigmoid' = output - outputpower2
    float out = sigmoid * (1 - sigmoid);
    //custom output value, valid in this specific case
    //couldnt handle the case were x1 and  x2
    //were greater than zero without using
    //some vectors and arrays. Thus, for the moment I cover it separately
    float res = 0;
    if (coord == 1)
    {	// litteraly the derivative according to the bias component;
        float w1 = I.Node1.weight2;
        float w2 = I.Node2.weight2;
        float output =  (-1.0f) * (x1* w1 + x2 * w2);
        res = 1/ (1 + exp(output));
    }
    if (coord == 2)
    {
        a = ffnode1(I, H, x1, 0);

    }
    if (coord == 3)
    {
        a = ffnode1(I, H, 0, x2);

    }
    res = ((-1.0f) * (yx + a) * out);

    if (x1 == x2 && x2!=0.0f)
    {
        res = (-1.0f) * res;
    }
    return res;
}


float operrorh2(InputLayer I, HiddenLayer H, float x1, float x2, int coord)
{
    // The output using the neuralnet, a
    float a = 0;
    // The wanted one, y(x)
    float yx = speand2(x1,x2);
    //We need the derivative of sigmoid of inputs
    //variable declaration	float w1 = NodeXor.weight;
    float w1 = I.Node1.weight2;
    float w2 = I.Node2.weight2;
    float bias = H.Node2.bias;
    float output = (-1.0f) * (x1 * w1 + x2 * w2 + bias);
    //Result of partial der of cost here
    float sigmoid = 1/(1 + exp(output));
    //sigmoid' = output - outputpower2
    float out = sigmoid * (1 - sigmoid);
    float res;
    //custom output value, valid in this specific case
    //couldnt handle the case were x1 and x2
    //were greater than zero without using
    //some vectors and arrays. Thus, for the moment I cover it separately
    if (coord == 1)
    {	// litteraly the derivative according to the bias component;
        //ffnode2 but bias =0;
        float w1 = I.Node1.weight2;
        float w2 = I.Node2.weight2;

        float output =  (-1.0f) * (x1* w1 + x2 * w2);
        res = 1/ (1 + exp(output));
    }
    if (coord == 2)
    {
        a = ffnode1(I, H, x1, 0);

    }
    if (coord == 3)
    {
        a = ffnode1(I, H, 0, x2);

    }
    res = ((-1.0f) * (yx + a) * out);

    if (x1 == x2 && x2!=0.0f)
    {
        res = (-1.0f) * res;
    }
    return res;
}


float operroro(HiddenLayer H, OutputLayer O, float x1, float x2, int coord)
{
    // The output using the neuralnet, a
    float a = 0;
    // The wanted one, y(x)
    float yx = xorfun(x1, x2);
    //We need the derivative of sigmoid of inputs
    //variable declaration	float w1 = NodeXor.weight;
    float w1 = H.Node1.weight;
    float w2 = H.Node2.weight;
    float bias = O.bias;
    float output = (-1.0f) * (x1 * w1 + x2 * w2 + bias);
    float sigmoid = 1/(1 + exp(output));
    //sigmoid' = output - outputpower2
    float out = sigmoid * (1 - sigmoid);
    float res;
    //custom output value, valid in this specific case
    //couldnt handle the case were x1 and x2
    //were greater than zero without using
    //some vectors and arrays. Thus, for the moment I cover it separately
    if (coord == 1)
    {	// litteraly the derivative according to the bias component;
        float w1 = H.Node1.weight;
        float x1 = H.Node1.value;
        float w2 = H.Node2.weight;
        float x2 = H.Node2.value;
        float output =  (-1.0f) * (x1* w1 + x2 * w2);
        res= 1/ (1 + exp(output));
    }
    if (coord == 2)
    {
        float w2 = H.Node2.weight;
        float x2 = H.Node2.weight;
        float bias = O.bias;
        float output =  (-1.0f) * ( x2 * w2 + bias);
        res= 1/ (1 + exp(output));
    }
    if (coord == 3)
    {
        float w1 = H.Node1.weight;
        float x1 = H.Node1.weight;
        float bias = O.bias;
        float output =  (-1.0f) * ( x1 * w1 + bias);
        res= 1/ (1 + exp(output));

    }
    res = ((-1.0f) * (yx + a) * out);

    if (x1 == x2 && x2!=0.0f)
    {
        res = (-1.0f) * res;
    }
    return res;
}


// Corrects the NeuralNetw Hidden layers
HiddenLayer stcout(HiddenLayer H, OutputLayer O,
        float rate, float x1, float x2)
{
    float er = operroro(H, O, x1, x2, 1);
    H.Node1.weight = H.Node1.weight - rate * er;
    H.Node2.weight = H.Node2.weight - rate * er;
    H.Node1.bias = H.Node1.bias - rate * er;
    H.Node2.bias = H.Node2.bias - rate * er;
    er = operroro(H, O, x1, x2, 2);
    H.Node1.weight = H.Node1.weight - rate * er;
    H.Node2.weight = H.Node2.weight - rate * er;
    H.Node1.bias = H.Node1.bias - rate * er;
    H.Node2.bias = H.Node2.bias - rate * er;
    er = operroro(H, O, x1, x2, 3);
    H.Node1.weight = H.Node1.weight - rate * er;
    H.Node2.weight = H.Node2.weight - rate * er;
    H.Node1.bias = H.Node1.bias - rate * er;
    H.Node2.bias = H.Node2.bias - rate * er;


    return H;
}


// Corrects the NeuralNetw rightmost layers
InputLayer stchid(InputLayer I, HiddenLayer H, float rate, float x1, float x2)
{
    float er1 = operrorh1(I, H, x1, x2, 1);
    float er2 = operrorh2(I, H, x1, x2, 1);
    I.Node1.weight1 = I.Node1.weight1 - rate * er1;
    I.Node1.weight2 = I.Node1.weight2 - rate * er1;
    I.Node2.weight1 = I.Node2.weight1 - rate * er2;
    I.Node2.weight2 = I.Node2.weight2 - rate * er2;
    er1 = operrorh1(I, H, x1, x2, 2);
    er2 = operrorh2(I, H, x1, x2, 2);
    I.Node1.weight1 = I.Node1.weight1 - rate * er1;
    I.Node1.weight2 = I.Node1.weight2 - rate * er1;
    I.Node2.weight1 = I.Node2.weight1 - rate * er2;
    I.Node2.weight2 = I.Node2.weight2 - rate * er2;
    er1 = operrorh1(I, H, x1, x2, 3);
    er2 = operrorh2(I, H, x1, x2, 3);
    I.Node1.weight1 = I.Node1.weight1 - rate * er1;
    I.Node1.weight2 = I.Node1.weight2 - rate * er1;
    I.Node2.weight1 = I.Node2.weight1 - rate * er2;
    I.Node2.weight2 = I.Node2.weight2 - rate * er2;

    return I;
}



// Modifies the weight of the NXN in the way that the cost is
// as low as possible
//Works only for a 3 layers neuralnet
NeuralXorNet stochastic(NeuralXorNet A, float x1, float x2)
{
    //The learning rate we use - to modify for tests.
    float rate = 0.3f;
    A.HiddenLayer = stcout(A.HiddenLayer, A.OutputLayer,  rate, x1, x2);
    A.InputLayer =  stchid(A.InputLayer, A.HiddenLayer, rate, x1, x2);
    return A;
}



// Trains the NeuralXorNet
// Prints the result
NeuralXorNet visualepoch(NeuralXorNet A)
{
    //
    //	NeuralXorNet B = stochastic(A,0.0f, 0.0f);
    //	A = B;
    //	printf("XOR 0-0 \nThe value of weight1 is %f
    //  \nThe value of weight2 is %f\nthe value of bias is %f\n",
    //			 A.Node1.weight, A.Node2.weight, A.bias);
    //	B  = stochastic(A, 0.0f, 1.0f);
    //	A = B;
    //	printf("XOR 0-1 \nThe value of weight1 is %f \n
    //          The value of weight2 is %f\n the value of bias is %f\n",
    //			 A.Node1.weight, A.Node2.weight, A.bias);
    //	B  = stochastic(A, 1.0f, 0.0f);
    //	A = B;
    //	printf("XOR 0-1 \nThe value of weight1 is %f \n
    //  The value of weight2 is %f\n the value of bias is %f\n",
    //			 A.Node1.weight, A.Node2.weight, A.bias);
    //	B  = stochastic(A, 1.0f, 1.0f);
    //	A = B;
    //	printf("XOR 1-1 \nThe value of weight1 is %f \nThe value of weight2
    //           is %f\n the value of bias is %f\n",
    //			 A.Node1.weight, A.Node2.weight, A.bias);


    NeuralXorNet B = stochastic(A,0.0f, 0.0f);
    A = B;
    B  = stochastic(A, 0.0f, 1.0f);
    A = B;
    B  = stochastic(A, 1.0f, 0.0f);
    A = B;
    B  = stochastic(A, 1.0f, 1.0f);
    A = B;

    float res1 = feedforward(A, 0.0f, 0.0f);
    A.InputLayer.Node1.value = 0.0f;
    A.InputLayer.Node2.value = 0.0f;
    A.HiddenLayer.Node1.value = 0.0f;
    A.HiddenLayer.Node2.value= 0.0f;
    A.OutputLayer.res = 0.0f;
    float res2 = feedforward(A, 1.0f, 0.0f);
    A.InputLayer.Node1.value = 0.0f;
    A.InputLayer.Node2.value = 0.0f;
    A.HiddenLayer.Node1.value = 0.0f;
    A.HiddenLayer.Node2.value= 0.0f;
    A.OutputLayer.res = 0.0f;
    float res3 = feedforward(A, 0.0f, 1.0f);
    A.InputLayer.Node1.value = 0.0f;
    A.InputLayer.Node2.value = 0.0f;
    A.HiddenLayer.Node1.value = 0.0f;
    A.HiddenLayer.Node2.value= 0.0f;
    A.OutputLayer.res = 0.0f;
    float res4 = feedforward(A, 1.0f, 1.0f);
    A.InputLayer.Node1.value = 0.0f;
    A.InputLayer.Node2.value = 0.0f;
    A.HiddenLayer.Node1.value = 0.0f;
    A.HiddenLayer.Node2.value= 0.0f;
    A.OutputLayer.res = 0.0f;
    printf("\tThe value of the neural net for 0 0 is %f \n",res1);
    printf("\tThe value of the neural net for 1 0 is %f \n",res2);
    printf("\tThe value of the neural net for 0 1 is %f \n",res3);
    printf("\tThe value of the neural net for 1 1 is %f \n",res4);
    return A;
}


// trains for a given short numer
NeuralXorNet trainepoch(NeuralXorNet A, short iteration)
{
    NeuralXorNet B = A;
    for (short i = 0; i < iteration; i++)
    {
        printf("epoch num[%hi]:\n",i);
        A = visualepoch(B);
        B = A;
    }

    return A;
}


//TOD0
//EPOCH UNTIL TRUE
//CAUTION
//MIGHT INFINITE LOOP

//CHECK FIRST THAT THE NN LEARNS BEFORE USING EPOCH
