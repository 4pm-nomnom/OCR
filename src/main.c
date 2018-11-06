#include <stdio.h>
#include <stdlib.h>
// #include "SDL/SDL.h" //included in image.h
// #include "SDL/SDL_image.h" //included in image.h
#include <gtk/gtk.h> // graphical user interfaces

#include "image.h"
#include "preprocessing.h"
#include "segmentation.h"

#define MAX_NUMBER_OF_LINES 100
#define MAX_NUMBER_OF_CHARACTERS 200

GtkImage *g_image_main;
GtkWidget *g_lbl_image_name;
GtkFileChooser *g_file_selection;
GtkWidget *g_text_result;

GtkWidget *window_main;
GtkWidget *window_about;
GtkWidget *window_file_selection;

int main(int argc, char *argv[])
{
    // --- GTK Window ------------------------------------------
    GtkBuilder *builder;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "gui/window_main.glade", NULL);

    // get pointers to windows
    window_main = GTK_WIDGET(
        gtk_builder_get_object(builder, "window_main"));
    window_about = GTK_WIDGET(
        gtk_builder_get_object(builder, "window_about"));
    window_file_selection = GTK_WIDGET(
        gtk_builder_get_object(builder, "window_file_selection"));
    gtk_builder_connect_signals(builder, NULL);

    // get pointers to objects
    g_image_main = GTK_IMAGE(
        gtk_builder_get_object(builder, "image_main"));
    g_lbl_image_name = GTK_WIDGET(
        gtk_builder_get_object(builder,"lbl_image_name"));
    g_file_selection = GTK_FILE_CHOOSER(
        gtk_builder_get_object(builder, "window_file_selection"));
    g_text_result = GTK_WIDGET(
        gtk_builder_get_object(builder, "text_result"));

    g_object_unref(builder);

    gtk_widget_show(window_main);
    gtk_main();


    //--- get argv (img_path) -----------------------------------
    if (argc < 2)
    {
        printf("You must provide an image file\n");
        return EXIT_FAILURE;
    }
    char *imgPath = calloc((strlen(argv[1])+1), sizeof(char));
    strcpy(imgPath, argv[1]);

    printf("==== Hello World! The OCR is starting ====\n");

    /************************************************************
     *                      Image Loading                        *
     * This step convert an image (png/jpg/bmp...) to a matrix.  *
     *************************************************************/
    printf("--- Image Loading ---\n");
    //--- SDL initialisation ------------------------------------
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;
    init_sdl();

    //--- Load image --------------------------------------------
    image_surface = load_image(imgPath);
    size_t img_width = image_surface->w;
    size_t img_height = image_surface->h;

    printf("[%s] has been loaded ",imgPath);
    printf("(width = %zu | height = %zu)\n", img_width, img_height);

    screen_surface = display_image(image_surface);
    wait_for_keypressed();

    /************************************************************
     *                   Image Pre-processing                    *
     * Process the image to make the character detection &       *
     * recognition easier.                                       *
     *************************************************************/
    //--- grayscale ---------------------------------------------
    printf("--- Turning the image from color to grayscale ---\n");
    grayscale(image_surface);

    update_surface(screen_surface, image_surface);
    screen_surface = display_image(image_surface);
    wait_for_keypressed();

    //--- Binarization ------------------------------------------
    printf("--- Binarization using otsu method ---\n");
    size_t otsuThreshold = otsu_threshold(image_surface);
    printf("otsu Threshold: %zu\n",otsuThreshold);
    binarize(image_surface, otsuThreshold);

    update_surface(screen_surface, image_surface);
    screen_surface = display_image(image_surface);
    wait_for_keypressed();

    //--- Reverse Binarization if text is white -----------------
    printf("--- Reverse Binarization if text is white ---\n");
    binarize_text_as_black(image_surface);

    update_surface(screen_surface, image_surface);
    screen_surface = display_image(image_surface);
    wait_for_keypressed();

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
    //--- bin-matrix creation -----------------------------------
    printf("--- text as a matrix ---\n");
    size_t *img_bin_matrix = malloc(sizeof(size_t)*img_height*img_width);
    for(size_t y=0; y<img_height; y++)
    {
        for(size_t x=0; x<img_width; x++)
        {
            Uint32 pixel = get_pixel(image_surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel,image_surface->format, &r, &g, &b);
            img_bin_matrix[y+x*img_height] = (r==0)?1:0;
        }
    }

    //--- Get Lines ---------------------------------------------
    printf("--- detecting lines ---\n");
    TextLine *textLines = calloc(MAX_NUMBER_OF_LINES, sizeof(TextLine));
    size_t nbTextLines = TextLines_ycut_find(textLines,
            img_bin_matrix,
            img_height,
            img_width);
    TextLines_show(textLines, nbTextLines);

    //--- Get Characters ----------------------------------------
    printf("--- detecting characters ---\n");
    for(size_t i=0; i < nbTextLines; i++)
    {
        Character *characters;
        characters = calloc(MAX_NUMBER_OF_CHARACTERS, sizeof(Character));
        textLines[i].Characters = characters;

        textLines[i].nbCharacters = Characters_find_quick_bounds(textLines[i],
                img_bin_matrix,
                img_width,
                img_height);
        printf("Line[%zu] -> nbCharacters detected : %zu\n",
                i, textLines[i].nbCharacters);
    }

    Surface_draw_textLines(image_surface, textLines, nbTextLines);

    update_surface(screen_surface, image_surface);
    screen_surface = display_image(image_surface);
    wait_for_keypressed();

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

    /************************************************************
     *                        Free Memory                        *
     *************************************************************/

    //characters
    free(img_bin_matrix);
    for(size_t i=0; i<nbTextLines; i++)
    {
        free(textLines[i].Characters);
    }
    //textlines
    free(textLines);

    // Free the image surface.
    SDL_FreeSurface(image_surface);

    // Free the screen surface.
    SDL_FreeSurface(screen_surface);

    return EXIT_SUCCESS;
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}

//--------------TOPBAR-MENU------------------------------
void on_menu_file_add_image_activate()
{
    gtk_widget_show(window_file_selection);
}

//...

void on_menu_file_quit_activate()
{
    gtk_main_quit();
}

void on_menu_tools_deskew_activate()
{
    //TODO
    
}

void on_menu_tools_spellchecker_activate()
{
    //TODO
    
}

void on_menu_tools_neural_network_activate()
{
    //TODO
    
}

void on_menu_help_help_activate()
{
    //TODO
    
}

void on_menu_help_about_activate()
{
    gtk_widget_show(window_about);
}


void on_btn_image_selection_clicked()
{
    gtk_widget_show(window_file_selection);
}


void on_btn_advanced_toggled()
{
    //TODO change button variable state
    
}

void on_btn_convert_clicked()
{
    //TODO CONVERTION OCR WOW
    
}


void on_btn_text_save_clicked()
{
    //TODO save current text contained in text_result
    
}

void on_btn_text_copy_clicked()
{
    //TODO copy to clipboard content of text_result
    
}

//-------------FILE-SELECTION-WINDOW--------------------
void on_btn_file_selection_cancel_clicked()
{
    gtk_widget_hide(window_file_selection);
}
void on_btn_file_selection_open_clicked()
{
    gchar *image_path = gtk_file_chooser_get_filename(g_file_selection);
    gtk_image_set_from_file(g_image_main, image_path);
    
    //TODO change lbl_image_name format
    gsize maxlength = 12;
    gchar *image_name = g_strreverse(image_path); //path is reversed too /care
    image_name = g_strndup(image_path, maxlength);
    image_name = g_strreverse(image_name);
    gchar *dots = "...";
    image_name = g_strconcat(dots, image_name, NULL);
    gtk_label_set_text(GTK_LABEL(g_lbl_image_name), image_name);
    gtk_widget_hide(window_file_selection);
}
//TODO add doubleclick selection // enter

//--------------ABOUT-WINDOW----------------------------
void on_window_about_close()
{
    gtk_widget_hide(window_about);
}
void on_btn_about_close_clicked()
{
    gtk_widget_hide(window_about);
}
