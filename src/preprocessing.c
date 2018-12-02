#include <stdio.h>
#include <stdlib.h>
#include "image.h" //already have SDL


void grayscale(SDL_Surface *image_surface)
{
    size_t width = image_surface->w;
    size_t height = image_surface->h;
    for(size_t x=0; x < width; x++)
    {
        for(size_t y=0; y < height; y++)
        {
            Uint32 pixel;
            pixel = get_pixel(image_surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel,image_surface->format, &r, &g, &b);
            Uint8 average = 0.3*r + 0.59*g + 0.11*b;
            r = average; g = average; b = average;
            pixel = SDL_MapRGB(image_surface->format, r, g, b);
            put_pixel(image_surface, x, y, pixel);
        }
    }
}

size_t otsu_threshold(SDL_Surface *image_surface) // on grayscale image_surface
{
    size_t width = image_surface->w;
    size_t height = image_surface->h;
    size_t total = height*width;
    float histogram[256] = { 0.0F };
    for(size_t x=0; x < width; x++)
    {
        for(size_t y=0; y < height; y++)
        {
            Uint32 pixel;
            pixel = get_pixel(image_surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel,image_surface->format, &r, &g, &b);
            int grayLevel = r;
            histogram[grayLevel] += 1;
        }
    }

    for (int i = 0; i < 256; i++)
        histogram[i] /= total;

    float ut = 0;
    for (int i = 0; i < 256; i++)
        ut += i*histogram[i];

    int max_k = 0;
    float max_sigma_k = 0;
    for (int k = 0; k < 256; k++)
    {
        float wk = 0;
        for (int i = 0; i <= k; i++)
            wk += histogram[i];
        float uk = 0;
        for (int i = 0; i <= k; i++)
            uk += i*histogram[i];

        float sigma_k = 0;
        if (wk != 0 && wk != 1)
            sigma_k = ((ut*wk - uk)*(ut*wk - uk)) / (wk*(1 - wk));
        if (sigma_k > max_sigma_k)
        {
            max_k = k;
            max_sigma_k = sigma_k;
        }
    }
    return (size_t)max_k;
}

void binarize(SDL_Surface *image_surface, size_t threshold)
{
    size_t width = image_surface->w;
    size_t height = image_surface->h;
    for(size_t x=0; x < width; x++)
    {
        for(size_t y=0; y < height; y++)
        {
            Uint32 pixel;
            pixel = get_pixel(image_surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel,image_surface->format, &r, &g, &b);
            size_t graylevel = r;
            if (graylevel < threshold)
                { r = 0; g = 0; b = 0; }
            else
                { r = 255; g = 255; b=255; }
            pixel = SDL_MapRGB(image_surface->format, r, g, b);
            put_pixel(image_surface, x, y, pixel);
        }
    }
}

void binarize_text_as_black(SDL_Surface *image_surface)
{
    size_t width = image_surface->w;
    size_t height = image_surface->h;
    // check if the first line have more black or white
    size_t nbBlack=0;
    for(size_t x=0; x < width; x++)
    {
        Uint32 pixel;
        pixel = get_pixel(image_surface, x, 0);
        Uint8 r, g, b;
        SDL_GetRGB(pixel,image_surface->format, &r, &g, &b);
        size_t color = r;
        if (color == 0)
            nbBlack++;
    }
    if (nbBlack >= width-nbBlack)
    {
        for(size_t x=0; x < width; x++)
        {
            for(size_t y=0; y < height; y++)
            {
                Uint32 pixel;
                pixel = get_pixel(image_surface, x, y);
                Uint8 r, g, b;
                SDL_GetRGB(pixel,image_surface->format, &r, &g, &b);
                size_t color = r;
                if (color == 255)
                    { r = 0; g = 0; b = 0; }
                else
                    { r = 255; g = 255; b=255; }
                pixel = SDL_MapRGB(image_surface->format, r, g, b);
                put_pixel(image_surface, x, y, pixel);
            }
        }
    }
}

void bin_matrix_from_surface(size_t *bin_matrix, SDL_Surface *image_surface)
{
    size_t img_width = image_surface->w;
    size_t img_height = image_surface->h;
    for(size_t y=0; y<img_height; y++)
    {
        for(size_t x=0; x<img_width; x++)
        {
            Uint32 pixel = get_pixel(image_surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel,image_surface->format, &r, &g, &b);
            bin_matrix[y*img_width+x] = (r==0)?1:0;
        }
    }
}

size_t *matrix_from_image_preprocessing(SDL_Surface *image_surface)
{
    size_t img_width = image_surface->w;
    size_t img_height = image_surface->h;

    //--- grayscale ---------------------------------------------
    grayscale(image_surface);
    
    Surface_save_image(image_surface, "tmp/grayscale.bmp");

    //--- Binarization ------------------------------------------
    binarize(image_surface, otsu_threshold(image_surface));
    
    //--- Reverse Binarization if text is white -----------------
    binarize_text_as_black(image_surface);

    Surface_save_image(image_surface, "tmp/binarized.bmp");

    //--- rotate the image if needed (De-skew) ------------------
    // automatic_rotation(img_matrix);

    //--- remove positive and negative spots (Despeckle) --------
    // noise_reduction(img_matrix);

    //--- line removal - cleans up non-glyph lines/boxes --------
    // lines_removal(img_matrix);


    //--- bin-matrix creation -----------------------------------
    size_t *img_bin_matrix = malloc(sizeof(size_t)*img_height*img_width);
    bin_matrix_from_surface(img_bin_matrix, image_surface);

    return img_bin_matrix;
}
