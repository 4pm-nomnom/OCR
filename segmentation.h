#ifndef SEGMENTATION_H
#define SEGMENTATION_H


void histogram_creation(size_t histogram[],
        size_t binarized_image[],
        size_t height, size_t width)

void histogram_show(size_t histogram[], size_t len)

size_t histogram_mean(size_t histogram[], size_t len)

size_t TextLines_from_histogram(TextLine textLines[],
        size_t histogram[], size_t len)

void TextLines_show(TextLine textLines[], size_t nbTextLines)

void Surface_draw_line(SDL_Surface *image_surface, size_t numLine)

void Surface_draw_textLines(SDL_Surface *image_surface,
        TextLine textLines[], size_t nbTextLines)

#endif
