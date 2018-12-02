#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

// Variables globales pour les differentes len du reseau


// Fonction pour remplir une matrice de valeur au hasard.
void fill_array_random(double** layer, size_t _i, size_t _j)
{
	for (size_t i = 0; i < _i; ++i)
		for (size_t j = 0; j < _j; ++j)
	{
			layer[i][j] = (double)  rand() / RAND_MAX * pow(-1, rand() % 2);
//			printf("weight[%lu][%lu] = %f\n", i, j, layer[i][j]);
		}
}

// Remplir la matrice de valeurs sauvegardees


// copier une matrice

void copy_array(double* arr, size_t len, double* copy)
{
	for (size_t i = 0; i < len; ++i)
		copy[i] = arr[i];
}

