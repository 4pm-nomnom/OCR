#ifndef SETTING_VARIABLES_H
#define SETTING_VARIABLES_H

const size_t nblayer = 2;
const size_t nbneurones[2] = {2, 1};
const size_t nbweights[2] = {3, 3};

void fill_array_random(double** layer, size_t _i, size_t _j);

double* copy_array(double* arr, size_t len);

#endif
