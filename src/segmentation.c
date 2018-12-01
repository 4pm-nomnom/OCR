#include <stdio.h>
#include "image.h"
#include "segmentation.h"

size_t TextLines_find(TextLine textLines[],
                            size_t binarized_image[],
                            size_t height, size_t width)
{
    size_t nbTextLines=0;
    int wasLine=0;
    size_t startingPoint=0;
    for(size_t y=0; y < height; y++)
    {
        size_t nbPixelOnThisLine=0;
        for(size_t x=0; x < width; x++)
        {
            nbPixelOnThisLine += binarized_image[y*width+x];
        }
        if (!wasLine && nbPixelOnThisLine)
        {
            startingPoint = y;
            wasLine = 1;
            continue;
        }
        if (wasLine && !nbPixelOnThisLine)
        {
            TextLine line;
            line.UpperBound = startingPoint;
            line.LowerBound = y;
            textLines[nbTextLines] = line;
            wasLine = 0;
            nbTextLines ++;
        }
    }

    // check if the last line was finished
    if (wasLine)
    {
        // add line coord to TextLines list
        TextLine line;
        line.UpperBound = startingPoint;
        line.LowerBound = height-1;
        textLines[nbTextLines] = line;
        nbTextLines ++;
    }

    return nbTextLines;
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

void Characters_find_bounds(TextLine *textLine,
        size_t binarized_image[],
        size_t img_width,
        size_t img_height)
{
    size_t totalSpace = 0;
    size_t upperBound = textLine->UpperBound;
    size_t lowerBound = textLine->LowerBound;
    size_t nbCharactersFound = 0;
    int wasChar=0;
    size_t startingPoint=0;
    (void)img_height;
    for(size_t x=0; x < img_width; x++)
    {
        size_t nbPixelOnThisLine = 0;
        for(size_t y=upperBound; y<=lowerBound; y++)
        {
            nbPixelOnThisLine += binarized_image[y*img_width+x];
        }
        if (!wasChar && nbPixelOnThisLine)
        {
            startingPoint = x;
            wasChar = 1;

            if (nbCharactersFound)
                totalSpace += startingPoint-textLine->Characters[nbCharactersFound - 1].RightBound;
            continue;
        }
        if (wasChar && !nbPixelOnThisLine)
        {
            Character character;
            character.LeftBound = startingPoint;
            character.RightBound = x;
            textLine->Characters[nbCharactersFound] = character;
            wasChar = 0;
            nbCharactersFound++;
        }
    }
    textLine->nbCharacters = nbCharactersFound;
    
    if (nbCharactersFound > 1)
        textLine->averageSpaceWidth = totalSpace/(nbCharactersFound-1);
}

void get_characters(TextLine *textLine,
        size_t binarized_image[],
        size_t img_width, size_t img_height)
{
    Characters_find_bounds(textLine, binarized_image, img_width, img_height);

    for(size_t j=0; j < textLine->nbCharacters; j++)
    {
        Character *currentChar = &(textLine->Characters[j]);
        size_t new_height = textLine->LowerBound - textLine->UpperBound;
        size_t new_width = currentChar->RightBound - currentChar->LeftBound;

        size_t *cropped = matrix_crop(binarized_image, img_height, img_width,
                textLine->UpperBound, currentChar->LeftBound,
                new_height, new_width);

        currentChar->matrix = normalize(cropped, new_height, new_width);
        matrix_print(currentChar->matrix, WANTED_SIZE, WANTED_SIZE);
    }
}

void Surface_draw_vline(SDL_Surface *image_surface, size_t numCol,
                        size_t upperBound, size_t lowerBound)
{
    for(size_t y=upperBound; y < lowerBound; y++)
    {
        Uint8 r=255, g=0, b=0;
        Uint32 pixel = SDL_MapRGB(image_surface->format, r, g, b);
        put_pixel(image_surface, numCol, y, pixel);
    }
}

void Surface_draw_hline(SDL_Surface *image_surface, size_t numLine)
{
    size_t width = image_surface->w;
    for(size_t x=0; x < width; x++)
    {
        Uint8 r=0, g=255, b=0;
        Uint32 pixel = SDL_MapRGB(image_surface->format, r, g, b);
        put_pixel(image_surface, x, numLine, pixel);
    }
}

void Surface_draw_textLines(SDL_Surface *image_surface,
                            TextLine textLines[], size_t nbTextLines)
{
    for(size_t i=0; i < nbTextLines; i++)
    {
        size_t upperBound = textLines[i].UpperBound;
        size_t lowerBound = textLines[i].LowerBound;
        Surface_draw_hline(image_surface, upperBound);
        Surface_draw_hline(image_surface, lowerBound);

        for(size_t j=0; j<textLines[i].nbCharacters; j++)
        {
            Character character = textLines[i].Characters[j];
            Surface_draw_vline(image_surface, character.LeftBound,
                                upperBound, lowerBound);
            Surface_draw_vline(image_surface, character.RightBound,
                                upperBound, lowerBound);
        }
    }
}

size_t *matrix_crop(size_t matrix[], size_t matrix_height, size_t matrix_width,
    size_t upperBound, size_t leftBound, size_t cropped_height, size_t cropped_width)
{
    size_t *cropped = calloc(cropped_height*cropped_width, sizeof(size_t));
    if (cropped_height+upperBound > matrix_height ||
        cropped_width+cropped_width > matrix_width)
        return NULL;
    for(size_t r=0; r < cropped_height; r++)
    {
        for(size_t c=0; c < cropped_width; c++)
        {
            cropped[r*cropped_width + c] = matrix[(upperBound+r)*matrix_width+(leftBound+c)];
        }
    }
    return cropped;
}


//----------------------------------------------------------------------------
//--NORMALIZATION
//this function put the matrix at the center of a square (larger)
void matrix_put_in_square(size_t matrix[], size_t square[],
    size_t height, size_t width, size_t square_size)
{
    size_t startRow = square_size/2 - height/2;
    size_t startCol = square_size/2 - width/2;

    for(size_t r=0; r < height; r++)
    {
        for(size_t c=0; c < width; c++)
        {
            square[square_size*(startRow+r)+(startCol+c)] = matrix[width*(r)+c];
        }
    }
}

//resize a square into a smaller one of size n*n (n being a divisoof )
void square_resize(size_t square[], size_t resized[],
    size_t square_size, size_t resized_size)
{
    size_t ratio = square_size/resized_size;
    for(size_t r=0; r<resized_size; r++)
    {
        for(size_t c=0; c<resized_size; c++)
        {
            size_t nbOne = 0;
            for (size_t i=r*ratio; i<(r+1)*ratio; i++)
            {
                for (size_t j=c*ratio; j<(c+1)*ratio; j++)
                {
                    if (square[square_size*i+j])
                        nbOne++;
                }
            }
            size_t value = (nbOne/ratio*ratio >= 0.4)? 1 : 0 ;
            resized[resized_size*r + c] = value;
        }
    }
}

//transform a matrix into a squared one of 16x16
size_t* normalize(size_t matrix[], size_t height, size_t width)
{
    size_t *result = calloc(WANTED_SIZE*WANTED_SIZE, sizeof(size_t));
    size_t square_size;
    square_size = (height>width)?
        height + (WANTED_SIZE - height%WANTED_SIZE):
        width + (WANTED_SIZE - width%WANTED_SIZE);

    size_t *square = calloc(square_size*square_size, sizeof(size_t));

    matrix_put_in_square(matrix, square, height, width, square_size);
    //matrix_print(square, square_size, square_size);

    square_resize(square, result, square_size, WANTED_SIZE);

    free(square);
    return result;
}

void matrix_print(size_t matrix[], size_t height, size_t width)
{
    printf("*");
    for(size_t i=0; i<width;i++)
        printf("-");
    printf("*\n");

    for(size_t r=0; r<height; r++)
    {
        printf("|");
        for(size_t c=0; c<width; c++)
        {
            if (matrix[width*r+c])
                printf("1");
            else
                printf(" ");
        }
        printf("|\n");
    }

    printf("*");
    for(size_t i=0; i<width;i++)
        printf("-");
    printf("*\n");
}
