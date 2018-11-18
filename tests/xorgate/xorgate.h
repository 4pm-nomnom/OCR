#ifndef XORGATE_H
#define XORGATE_H
#include "nodexor.h"


typedef struct InputLayer
{
    NodeXorDouble Node1, Node2;

} InputLayer;

typedef struct HiddenLayer
{
    NodeXor Node1, Node2;

} HiddenLayer;

typedef struct OutputLayer
{
    float res;
    float bias;

} OutputLayer;


typedef struct NeuralXorNet
{
    InputLayer InputLayer;
    HiddenLayer HiddenLayer;
    OutputLayer OutputLayer;

} NeuralXorNet;

float feedforward(NeuralXorNet A, float x1, float x2);
float ffnode1(InputLayer I, HiddenLayer H, float x1, float x2);
float ffnode2(InputLayer I, HiddenLayer H, float x1, float x2);
float ff(HiddenLayer H, OutputLayer O);
float fsquare(float x);
float costout(NeuralXorNet A, float x1, float x2);
float costhidden1(NeuralXorNet A, float x1, float x2);
float costhidden2(NeuralXorNet A, float x1, float x2);
float operroro(HiddenLayer H, OutputLayer O, float x1, float x2, int coord);
float operrorh1(InputLayer I, HiddenLayer H, float x1, float x2, int coord);
float operrorh2(InputLayer I, HiddenLayer H, float x1, float x2, int coord);
NeuralXorNet stochastic(NeuralXorNet A, float x1, float x2);
NeuralXorNet visualepoch(NeuralXorNet A);
NeuralXorNet trainepoch(NeuralXorNet A, short i);

#endif
