#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#define MAX_NUMBER_OF_LINES 1000
#define MAX_NUMBER_OF_CHARACTERS 200
#define WANTED_SIZE 16

typedef struct
{
    size_t LeftBound;
    size_t RightBound;
    size_t *matrix;
    char character;
}Character;

typedef struct
{
    size_t UpperBound;
    size_t LowerBound;
    size_t nbCharacters;
    size_t averageSpaceWidth;
    Character *Characters;
}TextLine;

typedef struct
{
    size_t UpperBound;
    size_t LowerBound;
    size_t RightBound;
    size_t LeftBound;
    size_t nbTextLines;
    TextLine *Textlines;
}TextBlock;

size_t TextLines_find(TextLine textLines[],
        size_t binarized_image[],
        size_t height, size_t width);

void TextLines_show(TextLine textLines[], size_t nbTextLines);

void Characters_find_bounds(TextLine *textLine,
        size_t binarized_image[],
        size_t img_width, size_t img_height);

void get_characters(TextLine *textLine,
        size_t binarized_image[],
        size_t img_width, size_t img_height);

void Surface_draw_vline(SDL_Surface *image_surface, size_t numCol,
        size_t upperBound, size_t lowerBound);

void Surface_draw_hline(SDL_Surface *image_surface, size_t numLine);

void Surface_draw_textLines(SDL_Surface *image_surface,
        TextLine textLines[], size_t nbTextLines);

//return a cropped matrix of the main one
//the user is responsible of freeing it
size_t *matrix_crop(size_t matrix[], size_t matrix_height, size_t matrix_width,
        size_t upperBound, size_t leftBound,
        size_t cropped_height, size_t cropped_width);

size_t* normalize(size_t matrix[], size_t height, size_t width);

void matrix_print(size_t matrix[], size_t height, size_t width);

/*
   void Surface_draw_rlsa(SDL_Surface *image_surface, ...)
   void

 */

#endif
