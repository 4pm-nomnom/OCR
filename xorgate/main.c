#include "xorgate.h"
#include "xorfun.h"
#include "nodexor.h"
#include <stdio.h>



int main()
{
	NodeXorDouble i1 = {-10.0f, 10.0f, 0.0f};
	NodeXorDouble i2 = {-10.0f, 10.0f, 0.0f};
	NodeXor a = {-10.0f, 0.0f, 5.0f};
	NodeXor b = {-10.0f, 0.0f, -15.0f};
	InputLayer I = {i1, i2};
	HiddenLayer H = {a, b};
	OutputLayer O = {0.0f, 5.0f};
	NeuralXorNet N= {I, H, O};
	trainepoch(N, 1000);
	return 0;	
}
