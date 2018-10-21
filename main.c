#include <stdio.h>
// #include "SDL/SDL.h" //included in image.h
// #include "SDL/SDL_image.h" //included in image.h
// #include <gtk/gtk.h> // graphical user interfaces

#include "image.h"
// #include "preprocessing.h"
// #include "character_detection.h"
// #include "character_recognition.h"
// #include "postprocessing.h"
// #include "graphical_interface.h"


int main(int argc, char *argv[])
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
    char img_path[] = "my_image.jpg";
    image_surface = load_image(img_path);
   
    int img_width = image_surface->w;
    int img_height = image_surface->h;

    
    //??? Convert image to a matrix (grey scale) ----------------
    //convert_to_matrix(img_SDL, img_matrix)
    
    //--- Possibility to save an image --------------------------
    //save_image_as(img_matrix, creation_path)
    
    /************************************************************
    *                   Image Pre-processing                    *
    * Process the image to make the character detection &       *
    * recognition easier.                                       *
    *************************************************************/
    //--- convert color of the matrix from grey-scale to 0 or 1 -
    // binarisation(img_matrix) //!\ threshold depending on the image
    
    //--- rotate the image if needed (De-skew) ------------------
    // automatic_rotation(img_matrix);
    
    //--- remove positive and negative spots (Despeckle) --------
    // noise_reduction(img_matrix);
    
    //--- line removal - cleans up non-glyph lines/boxes --------
    // lines_removal(img_matrix);
    
    /************************************************************
    *                   Character Detection                     *
    * In this section the layout will be analysed. This means   *
    * the segmentation of blocks/lines/words/characters.        *
    *************************************************************/
    //--- Characters segmentation -------------------------------
    
    
    //--- Normalise characters (aspect ratio / scale) -----------
    
    
    /************************************************************
    *                  Character Recognition                    *
    * Convert the input matrix (representing a character) to an *
    * ASCII                                                     *
    *************************************************************/
    //--- possibility to initialize a neural network ------------
    // neural_network_init(...)
    // when init the network, use random value instead of nothing
    // neural_network_from_source(source ...)
    
    //--- possibility to train the neural network ---------------
    // training(input, wanting_output)
    
    //--- get the estimated output for a specific input ---------
    // get_result(input_character)
    
    /************************************************************
    *                     Post-processing                       *
    * Possible checking of the words by using a dictionnary or  *
    * a lexicon - a list of words that are allowed to occur     *
    *************************************************************/
   

    // Free the image surface.
    SDL_FreeSurface(image_surface);
    
    // Free the screen surface.
    SDL_FreeSurface(screen_surface);
 
    return EXIT_SUCCESS;
}
