#include <stdio.h>
// #include <SDL2/SDL.h> // <SDL.h> ?
// #include <SDL2/SDL_image.h>  // <SDL_image.h> ? // used to load the image
// #include <gtk/gtk.h> // graphical user interfaces

// #include "image_loading.h"
// #include "preprocessing.h"
// #include "character_detection.h"
// #include "character_recognition.h"
// #include "postprocessing.h"
// #include "graphical_interface.h"


int main(int argc, char *argv[])
{
    printf("Hello World! The OCR is strating\n");
    
    /************************************************************
    *                      Image Loading                        *
    * This step convert an image (png/jpg/bmp...) to a matrix.  *
    *************************************************************/
    //--- Load image --------------------------------------------
    
    
    //--- Convert image to a matrix (grey scale)-----------------
    
    
    /************************************************************
    *                   Image Pre-processing                    *
    * Process the image to make the character detection &       *
    * recognition easier.                                       *
    *************************************************************/
    //--- convert color of the matrix from grey-scale to 0 or 1 -
    // binarisation(matrix) //! threshold depending on the image
    
    //--- rotate the image if needed (De-skew) ------------------
    // automatic_rotation(matrix)
    
    //--- remove positive and negative spots (Despeckle) --------
    // noise_reduction()
    
    //--- line removal - cleans up non-glyph lines/boxes --------
    
    
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
    //__init__ // when init the network, random value instead of nothing 
    
    
    /************************************************************
    *                     Post-processing                       *
    * Possible checking of the words by using a dictionnary     *
    *************************************************************/
    
    
    return EXIT_SUCCESS;
}
