#ifndef XORGATE_H
#define XORGATE_H

float fsquare(float x);
float xorgate(float x1, float x2, float w1, float w2, float bias);
float cost(float x1, float x2, float w1, float w2, float bias);
int rosenberg (float x1, float x2, float w1, float w2, float bias);
float[] train(float w1, float w2, float b);

#endif

