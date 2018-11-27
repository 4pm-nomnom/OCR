#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

// Fonction pour remplir une matrice de valeur au hasard.
void fill_array_random(double** layer, size_t _i, size_t _j)
{
	for (size_t i = 0; i < _i; ++i)
    	for (size_t j = 0; j < _j; ++j)
        	layer[i][j] = (double) rand() / (double) RAND_MAX;
}

