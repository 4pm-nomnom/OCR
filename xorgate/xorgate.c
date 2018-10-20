#include "xorgate.h"
#include "xorfun.h"
#include <stdio.h>


//compute the output of a perceptron node using the rosenblat method
int rosenblate(float x1, float x2, float w1, float w2, float b)
{
	//the control value
	float threshhold = 0.0f;
	// the computation of output should be made with a loop for a list of xi wi
	float output = x1 * w1 + b  x2 * w2 ;
	if (output > treshhold)
	// we are satisfied of the test
		{return 1;}
	//if output < treshhold
	return 0;
}




//compute the sum of product with the bias
float xorgate(float x1, float x2, float w1, float w2, float bias)
{
	float output = x1* w1 + x2 * w2 + b;
	return output;
}





//a floating square
float fsquare(float x1)
{
	return x1*x1;
}

float cost(float x1, float x2, float weight1, float weight2, float bias)
{

	//the awaited value for xor of the inputs
	float xor = xorfun(x1, x2);
	// the value we add
	float res = xorgate(x1, x2, w1, w2, bias);
	// we want to modify the weight and bias to be closer from the xor values
	float cost = 0.25f * fsquare(xor-res);
	if (xor == 0)
	// the xor value is higher than the xor one, so we revert cost
	{cost = (-1) * cost;}
	return cost;
}


// iterates cost with all the possible case of the xor gate to update them
float[] train(float w1, float w2, float b)
{
	cost0 = cost(0.0f,0.0f, w1, w2, b)
	w1 = w1 + cost0;
	w2 = w2 + cost0;
	b = b + cost0;
	cost1 =cost(1.0f,0.0f,w1,w2,b)
	w1 = w1 + cost1;
	w2 = w2 + cost1;
	b = b+ cost1;
	cost2= cost(0.0f,1.0f,w1,w2,b)
	w1 = w1 + cost2;
	w2 = w2 + cost2;
	b = b + cost2;
	cost3 = cost(1.0f,1.0f, w1,w2,b)
	w1 = w1 + cost3;
	w2 = w2 + cost3;
	b = b + cost3;
	return {w1, w2, b, 0}
}
