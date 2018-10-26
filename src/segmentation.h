#ifndef SEGMENTATION_H
#define SEGMENTATION_H

typedef struct
{
    size_t LeftBound;
    size_t RightBound;
    size_t *matrix; // = malloc(sizeof(size_t)*img_height*img_width);
}Character;

typedef struct
{
    size_t UpperBound;
    size_t LowerBound;
    size_t nbCharacters;
    Character *Characters;
}TextLine;

size_t TextLines_ycut_find(TextLine textLines[],
                            size_t binarized_image[],
                            size_t height, size_t width);

void TextLines_show(TextLine textLines[], size_t nbTextLines);

size_t Characters_find_quick_bounds(TextLine textLine,
        size_t binarized_image[],
        size_t img_width,
        size_t img_height);

void Surface_draw_vline(SDL_Surface *image_surface, size_t numCol,
                        size_t upperBound, size_t lowerBound);

void Surface_draw_hline(SDL_Surface *image_surface, size_t numLine);

void Surface_draw_textLines(SDL_Surface *image_surface,
                            TextLine textLines[], size_t nbTextLines);


#endif
