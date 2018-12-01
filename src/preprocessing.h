#ifndef PREPROCESSING_H
#define PREPROCESSING_H

void grayscale(SDL_Surface *image_surface);

size_t otsu_threshold(SDL_Surface *image_surface); //on grayscale image_surface

void binarize(SDL_Surface *image_surface, size_t threshold);

void binarize_text_as_black(SDL_Surface *image_surface);

void bin_matrix_from_surface(size_t *bin_matrix, SDL_Surface *image_surface);

size_t *matrix_from_image_preprocessing(SDL_Surface *image_surface);

#endif
