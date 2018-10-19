#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "image.h"

#define MAX_NUMBER_OF_LINES 100

typedef struct
{
    size_t UpperBound;
    size_t LowerBound;
}TextLine;

void histogram_creation(size_t histogram[],
        size_t binarized_image[],
        size_t height, size_t width)
{
    for(size_t y=0; y < height; y++)
    {
        size_t nbPixelInLine=0;
        for(size_t x=0; x < width; x++)
        {
            nbPixelInLine += binarized_image[y+x*height];
        }
        histogram[y] = nbPixelInLine;
    }
}

void histogram_show(size_t histogram[], size_t len)
{
    printf("--- showig histogram ---\n"); 
    for(size_t i=0; i<len; i++)
    {
        printf("histogram[%zu] = %zu\n", i, histogram[i]);
    }
}

size_t histogram_mean(size_t histogram[], size_t len)
{
    size_t sum = 0;
    for(size_t i=0; i<len; i++)
    {
        sum += histogram[i];
    }
    return sum/len;
}

size_t TextLines_from_histogram(TextLine textLines[],
        size_t histogram[], size_t len)
{
    size_t mean = histogram_mean(histogram, len);
    size_t i=0, numTextLine=0;

    int isMiddlePartOfLine = 0, isUpperPartOfLine = 0, isLowerPartOfLine = 0;
    size_t upperBound;
    if (histogram[0] != 0)
    {
        isUpperPartOfLine = 1;
        upperBound = 0;
    }

    for(i=0; i < len; i++)
    {
        if (isUpperPartOfLine)
        {
            if (histogram[i] >= mean)
            {
                isUpperPartOfLine = 0;
                isMiddlePartOfLine = 1;
            }
            continue;
        }
        if (isMiddlePartOfLine)
        {
            if (histogram[i] <= mean)
            {
                isMiddlePartOfLine = 0;
                isLowerPartOfLine = 1;
            }
            continue;    
        }
        if (isLowerPartOfLine)
        {
            if (histogram[i] == 0)
            {
                isLowerPartOfLine = 0;
                // add line coord to TextLines list
                TextLine line; 
                line.UpperBound = upperBound;
                line.LowerBound = i;
                textLines[numTextLine] = line;
                numTextLine ++;
            }
            continue;
        }
        //searching for upperBound of a new line
        if (histogram[i] != 0)
        {
            upperBound = i;
            isUpperPartOfLine = 1;
        }
    }
    // check if the last line was finished
    if (isUpperPartOfLine || isMiddlePartOfLine || isLowerPartOfLine)
    {
        // add line coord to TextLines list
        TextLine line; 
        line.UpperBound = upperBound;
        line.LowerBound = i-1;
        textLines[numTextLine] = line;
        numTextLine ++;
    }

    return numTextLine;
}

void TextLines_show(TextLine textLines[], size_t nbTextLines)
{
    printf("--- showing textLines ---\n");
    for(size_t i=0; i<nbTextLines; i++)
    {
        printf("line[%zu] = {%zu -> %zu}\n",
                i, textLines[i].UpperBound, textLines[i].LowerBound);
    }
}

void Surface_draw_line(SDL_Surface *image_surface, size_t numLine)
{
    size_t width = image_surface->w;
    for(size_t y=0; y < width; y++)
    {
        Uint8 r=255, g=0, b=0;
        Uint32 pixel = SDL_MapRGB(image_surface->format, r, g, b);
        put_pixel(image_surface, y, numLine, pixel);
    }
}

void Surface_draw_textLines(SDL_Surface *image_surface,
        TextLine textLines[], size_t nbTextLines)
{
    for(size_t i=0; i < nbTextLines; i++)
    {
        Surface_draw_line(image_surface, textLines[i].UpperBound);
        Surface_draw_line(image_surface, textLines[i].LowerBound);
    }
}

int main()
{
    printf("Hello World! The OCR is starting\n");

    /************************************************************
     *                      Image Loading                        *
     * This step convert an image (png/jpg/bmp...) to a matrix.  *
     *************************************************************/
    //--- SDL initialisation ------------------------------------
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;
    init_sdl();

    //--- Load image --------------------------------------------
    char img_path[] = "hey.png";
    image_surface = load_image(img_path);

    size_t img_width = image_surface->w ;
    size_t img_height = image_surface->h ;
    printf("width = %zu // height = %zu\n", img_width, img_height);

    screen_surface = display_image(image_surface);
    wait_for_keypressed();

    /************************************************************
     *                      Segmentation                         *
     *************************************************************/
    //--- Binarisation ------------------------------------------
    size_t *img_bin_matrix = malloc(sizeof(size_t)*img_height*img_width);
    for(size_t y=0; y<img_height; y++)
    {
        for(size_t x=0; x<img_width; x++)
        {
            Uint32 pixel = get_pixel(image_surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel,image_surface->format, &r, &g, &b);
            Uint8 average = 0.3*r + 0.59*g + 0.11*b;
            img_bin_matrix[y+x*img_height] = (average <= 255/2)?1:0;
            //printf("%zu",img_bin_matrix[y+x*img_height]);
        }
        //printf("\n");
    }

    //--- Histogram ---------------------------------------------
    size_t *histogram_lines = calloc(img_height, sizeof(size_t));
    printf("--- starting histogram creation ---\n");
    histogram_creation(histogram_lines, img_bin_matrix,
            img_height, img_width);
    histogram_show(histogram_lines, img_height);

    size_t mean = histogram_mean(histogram_lines, img_height);
    printf("mean = %zu\n", mean);

    //--- Get Lines ---------------------------------------------
    TextLine *textLines = calloc(MAX_NUMBER_OF_LINES, sizeof(TextLine));
    size_t nbTextLines = TextLines_from_histogram(textLines,
            histogram_lines, img_height);
    TextLines_show(textLines, nbTextLines);

    Surface_draw_textLines(image_surface, textLines, nbTextLines);
    update_surface(screen_surface, image_surface); 
    screen_surface = display_image(image_surface);
    wait_for_keypressed();

    //--- Free memory -------------------------------------------
    free(histogram_lines); 
    free(img_bin_matrix);
    // Free the image surface.
    SDL_FreeSurface(image_surface);
    // Free the screen surface.
    SDL_FreeSurface(screen_surface);

    return 0;
}
