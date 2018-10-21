#include <stdio.h>
#include "image.h"

#define MAX_NUMBER_OF_LINES 100
#define MAX_NUMBER_OF_CHARACTERS 200

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
            nbPixelOnThisLine += binarized_image[y+x*height];
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

size_t Characters_find_quick_bounds(TextLine textLine,
        size_t binarized_image[],
        size_t img_width,
        size_t img_height)
{
    size_t upperBound = textLine.UpperBound;
    size_t lowerBound = textLine.LowerBound;
    size_t nbCharactersFound = 0;
    int wasChar=0;
    size_t startingPoint=0;
    for(size_t x=0; x < img_width; x++)
    {
        size_t nbPixelOnThisLine = 0;
        for(size_t y=upperBound; y<=lowerBound; y++)
        {
            nbPixelOnThisLine += binarized_image[y+x*img_height];
        }
        if (!wasChar && nbPixelOnThisLine)
        {
            startingPoint = x;
            wasChar = 1;
            continue;
        }
        if (wasChar && !nbPixelOnThisLine)
        {
            Character character;
            character.LeftBound = startingPoint;
            character.RightBound = x;
            textLine.Characters[nbCharactersFound] = character;
            wasChar = 0;
            nbCharactersFound++;
        }
    }
    return nbCharactersFound;
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

int main()
{
    printf("Hello World! The OCR segmentation is starting\n");

    /************************************************************
     *                      Image Loading                        *
     * This step convert an image (png/jpg/bmp...) to a matrix.  *
     *************************************************************/
    //--- SDL initialisation ------------------------------------
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;
    init_sdl();

    //--- Load image --------------------------------------------
    char img_path[] = "samples/lines.png";
    image_surface = load_image(img_path);

    size_t img_width = image_surface->w ;
    size_t img_height = image_surface->h ;
    printf("[%s] was loaded\n", img_path);
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

    //--- Get Lines ---------------------------------------------
    TextLine *textLines = calloc(MAX_NUMBER_OF_LINES, sizeof(TextLine));
    size_t nbTextLines = TextLines_ycut_find(textLines,
                                                    img_bin_matrix,
                                                    img_height,
                                                    img_width);
    TextLines_show(textLines, nbTextLines);

    //--- Get Characters ----------------------------------------
    for(size_t i=0; i < nbTextLines; i++)
    {
        Character *characters;
        characters = calloc(MAX_NUMBER_OF_CHARACTERS, sizeof(Character));
        textLines[i].Characters = characters;

        textLines[i].nbCharacters = Characters_find_quick_bounds(textLines[i],
                                                                img_bin_matrix,
                                                                img_width,
                                                                img_height);
        printf("nbCharacters detected : %zu\n", textLines[i].nbCharacters);
    }

    Surface_draw_textLines(image_surface, textLines, nbTextLines);
    update_surface(screen_surface, image_surface); 
    screen_surface = display_image(image_surface);
    wait_for_keypressed();

    //--- Free memory -------------------------------------------
    free(img_bin_matrix);
    for(size_t i=0; i<nbTextLines; i++)
    {
        free(textLines[i].Characters);
    }
    free(textLines);
    SDL_FreeSurface(image_surface); 
    SDL_FreeSurface(screen_surface); 

    return 0;
}
