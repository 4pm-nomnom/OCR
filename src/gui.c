#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // used for sleep (to show loading page <3)
// #include "SDL/SDL.h" //included in image.h
// #include "SDL/SDL_image.h" //included in image.h
#include <gtk/gtk.h> // graphical user interfaces
#include "glib/gprintf.h"
#include <gspell/gspell.h>

#include "image.h"
#include "preprocessing.h"
#include "segmentation.h"

#define MAX_NUMBER_OF_LINES 100
#define MAX_NUMBER_OF_CHARACTERS 200

GtkImage *g_image_main;
GtkWidget *image_box;
GdkPixbuf *pixbuf;
GtkWidget *g_lbl_image_name;
GtkFileChooser *g_file_selection;
GtkFileChooser *g_file_save;
GtkTextView *g_text_result;
GspellNavigator *g_spell_navigator;

GtkToggleButton *cb_advanced;
GtkToggleButton *cb_show_pre_processing;
GtkToggleButton *cb_show_segmentation;
GtkToggleButton *rb_spell_check_en;
GtkToggleButton *rb_spell_check_fr;
GtkToggleButton *rb_spell_check_disable;

GtkWidget *window_main;
GtkWidget *window_about;
GtkWidget *window_file_selection;
GtkWidget *window_file_save;
GtkWidget *window_advanced;
GtkWidget *window_loading;
GtkWidget *window_nn;

GtkBuilder *builder;

gboolean isactive_advanced;
gboolean isactive_show_preprocessing;
gboolean isactive_show_segmentation;
gboolean isactive_spell_check_en;
gboolean isactive_spell_check_fr;
int zoom_bestfit = 1;
int zoom_largefit = 0;
int zoom_normal = 0;

void gchar_to_text_view(GtkTextView *text_view, gchar *text);

void launch_ocr(int argc, char *argv[])
{
    // --- GTK Window ------------------------------------------
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "gui/window_main.glade", NULL);

    // get pointers to windows
    window_main = GTK_WIDGET(
            gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    // get pointers to objects
    g_image_main = GTK_IMAGE(
            gtk_builder_get_object(builder, "image_main"));
    image_box = GTK_WIDGET(
            gtk_builder_get_object(builder, "image_box"));
    g_lbl_image_name = GTK_WIDGET(
            gtk_builder_get_object(builder,"lbl_image_name"));
    g_text_result = GTK_TEXT_VIEW(
            gtk_builder_get_object(builder, "text_result"));
    cb_advanced = GTK_TOGGLE_BUTTON(
            gtk_builder_get_object(builder, "cb_advanced"));

    //initialize inline gspell for the textView
    GspellTextView *gspell_view;
    gspell_view = gspell_text_view_get_from_gtk_text_view (g_text_result);
    gspell_text_view_basic_setup (gspell_view);

    gchar_to_text_view(g_text_result, "Welcome to the OCR by 4pm !");

    pixbuf = gdk_pixbuf_new_from_file("docs/logo_4pm.png", NULL);
    gtk_image_set_from_pixbuf(g_image_main, pixbuf);

    gtk_widget_show(window_main);
    gtk_main();

}

//-----------------------------------------------------------------------------
//--MAIN-FUNCTION-CONVERT
int convert()
{
    //--- get argv (img_path) -----------------------------------
    /*
    if (argc < 2)
    {
        printf("You must provide an image file\n");
        return EXIT_FAILURE;
    }
    char *imgPath = calloc((strlen(argv[1])+1), sizeof(char));
    strcpy(imgPath, argv[1]);
    */
    char *imgPath = "samples/hey.png";

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
            img_bin_matrix[y*img_width+x] = (r==0)?1:0;
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
    //wait_for_keypressed();

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

// called when main_window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}

gchar* gchar_from_text_view(GtkTextView *text_view)
{
    GtkTextBuffer *textBuffer;
    textBuffer = gtk_text_view_get_buffer(text_view);

    gchar *text_content;
    GtkTextIter start;
    GtkTextIter end;
    gtk_text_buffer_get_start_iter(textBuffer, &start);
    gtk_text_buffer_get_end_iter(textBuffer, &end);

    text_content = gtk_text_buffer_get_text(textBuffer, &start, &end, TRUE);
    return text_content;
}

void gchar_to_text_view(GtkTextView *text_view, gchar *text)
{
    GtkTextBuffer *textBuffer;
    textBuffer = gtk_text_view_get_buffer(text_view);
    gtk_text_buffer_set_text(textBuffer, text, strlen(text));
}

void on_window_main_size_allocate()
{
    GtkAllocation allocation;
    gtk_widget_get_allocation(
        image_box,
        &allocation);
    int desired_width = allocation.width;
    int desired_height = allocation.height;
    float r_box = (float)desired_height/desired_width;
    float r_image = (float)gdk_pixbuf_get_height(pixbuf)/
        gdk_pixbuf_get_width(pixbuf);
    if (zoom_bestfit)
    {
        if (r_box > r_image)
        {
            desired_width -= 4;
            desired_height = (int)(desired_width * r_image);
        }
        else
        {
            desired_height -= 4;
            desired_width = (int)(desired_height / r_image);
        }
        gtk_image_set_from_pixbuf(g_image_main, gdk_pixbuf_scale_simple(pixbuf,
            desired_width, desired_height, GDK_INTERP_BILINEAR));
    }
    else if (zoom_largefit)
    {
        if (r_box < r_image)
        {
            desired_width -= 4;
            desired_height = (int)(desired_width * r_image);
        }
        else
        {
            desired_height -= 4;
            desired_width = (int)(desired_height / r_image);
        }
        gtk_image_set_from_pixbuf(g_image_main, gdk_pixbuf_scale_simple(pixbuf,
            desired_width, desired_height, GDK_INTERP_BILINEAR));
    }
    else if (zoom_normal)
        gtk_image_set_from_pixbuf(g_image_main, pixbuf);
    gtk_widget_queue_resize(window_main);
}

//-----------------------------------------------------------------------------
//--LOADING-WINDOW
void window_loading_create()
{
    gtk_builder_add_from_file (builder, "gui/window_main.glade", NULL);
    window_loading = GTK_WIDGET(
            gtk_builder_get_object(builder, "window_loading"));
    gtk_widget_show(window_loading);
}

//-----------------------------------------------------------------------------
//--FILE-SELECTION-WINDOW
void window_file_selection_create()
{
    if (window_file_selection)
        gtk_widget_destroy(window_file_selection);
    gtk_builder_add_from_file (builder, "gui/window_main.glade", NULL);
    gtk_builder_connect_signals(builder, NULL);
    g_file_selection = GTK_FILE_CHOOSER(
            gtk_builder_get_object(builder, "window_file_selection"));
    window_file_selection = GTK_WIDGET(
            gtk_builder_get_object(builder, "window_file_selection"));
    gtk_widget_show(window_file_selection);

    //g_object_unref(builder);
}

void on_btn_file_selection_cancel_clicked()
{
    gtk_widget_destroy(window_file_selection);
}

void on_btn_file_selection_open_clicked()
{
    zoom_bestfit = 1;
    zoom_largefit = 0;
    zoom_normal = 0;

    gchar *image_path = gtk_file_chooser_get_filename(g_file_selection);
    pixbuf = gdk_pixbuf_new_from_file(image_path, NULL);

    on_window_main_size_allocate();

    //TODO change lbl_image_name format
    gsize maxlength = 12;
    gchar *image_name = g_strreverse(image_path); //path is reversed too /care
    image_name = g_strndup(image_path, maxlength);
    image_name = g_strreverse(image_name);
    gchar *dots = "...";
    image_name = g_strconcat(dots, image_name, NULL);
    gtk_label_set_text(GTK_LABEL(g_lbl_image_name), image_name);
    gtk_widget_destroy(window_file_selection);
}

void on_window_file_selection_file_activated(GtkFileChooser *chooser,
        gpointer data)
{
    (void)chooser;
    (void)data;
    on_btn_file_selection_open_clicked();
}

//-----------------------------------------------------------------------------
//--ABOUT-WINDOW
void window_about_create()
{
    if (window_about)
        gtk_widget_destroy(window_about);
    gtk_builder_add_from_file (builder, "gui/window_main.glade", NULL);
    gtk_builder_connect_signals(builder, NULL);
    window_about = GTK_WIDGET(
            gtk_builder_get_object(builder, "window_about"));
    gtk_widget_show(window_about);

    //g_object_unref(builder);
}

void on_btn_about_close_clicked()
{
    gtk_widget_destroy(window_about);
}

//-----------------------------------------------------------------------------
//--FILE-SAVE-WINDOW
void window_file_save_create()
{
    if (window_file_save)
        gtk_widget_destroy(window_file_save);

    gtk_builder_add_from_file (builder, "gui/window_main.glade", NULL);
    gtk_builder_connect_signals(builder, NULL);
    g_file_save = GTK_FILE_CHOOSER(
            gtk_builder_get_object(builder, "window_file_save"));

    window_file_save = GTK_WIDGET(
            gtk_builder_get_object(builder, "window_file_save"));

    gtk_file_chooser_set_do_overwrite_confirmation (g_file_save, TRUE);
    gtk_file_chooser_set_current_name (g_file_save, "ocr_result.txt");
    gtk_widget_show(window_file_save);

    //g_object_unref(builder);
}

void on_btn_file_save_cancel_clicked()
{
    gtk_widget_destroy(window_file_save);
}

void on_btn_file_save_save_clicked()
{
    //gchar *save_path = gtk_editable_get_chars (g_entry_file_save_name, 0, -1);
    char *filename;
    filename = gtk_file_chooser_get_filename(g_file_save);
    FILE *fs;
    fs = fopen(filename, "w");
    gchar *result_text = gchar_from_text_view(g_text_result);
    fprintf(fs, "%s", result_text);
    g_free(filename);
    fclose(fs);
    gtk_widget_destroy(window_file_save);
}

void on_window_file_save_file_activated(GtkFileChooser *chooser,
        gpointer data)
{
    (void)chooser;
    (void)data;
    on_btn_file_save_save_clicked();
}

//-----------------------------------------------------------------------------
//--ADVANCED-CONVERT-WINDOW
void window_advanced_create()
{
    if (window_advanced)
        gtk_widget_destroy(window_advanced);

    gtk_builder_add_from_file (builder, "gui/window_main.glade", NULL);
    gtk_builder_connect_signals(builder, NULL);
    window_advanced = GTK_WIDGET(
            gtk_builder_get_object(builder, "window_advanced"));

    cb_show_pre_processing = GTK_TOGGLE_BUTTON(
            gtk_builder_get_object(builder, "cb_show_pre_processing"));
    cb_show_segmentation = GTK_TOGGLE_BUTTON(
            gtk_builder_get_object(builder, "cb_show_segmentation"));

    rb_spell_check_en = GTK_TOGGLE_BUTTON(
            gtk_builder_get_object(builder, "rb_spell_check_en"));
    rb_spell_check_fr = GTK_TOGGLE_BUTTON(
            gtk_builder_get_object(builder, "rb_spell_check_fr"));
    rb_spell_check_disable = GTK_TOGGLE_BUTTON(
            gtk_builder_get_object(builder, "rb_spell_check_disable"));
    gtk_toggle_button_set_active (rb_spell_check_en, TRUE);
    gtk_toggle_button_set_active (rb_spell_check_fr, FALSE);
    gtk_toggle_button_set_active (rb_spell_check_disable, FALSE);

    gtk_widget_show(window_advanced);
}

void on_btn_advanced_cancel_clicked()
{
    gtk_widget_destroy(window_advanced);
}

void on_btn_advanced_convert_clicked()
{
    printf("start image convertion with advanced settings\n");
    gtk_widget_destroy(window_advanced);
}

void on_cb_show_pre_processing_toggled()
{
    isactive_show_preprocessing = gtk_toggle_button_get_active(cb_show_pre_processing);
}
void on_cb_show_segmentation_toggled()
{
    isactive_show_segmentation = gtk_toggle_button_get_active(cb_show_segmentation);
}
void on_rb_spell_check_toggled()
{
   isactive_spell_check_en = gtk_toggle_button_get_active(rb_spell_check_en);
   isactive_spell_check_fr = gtk_toggle_button_get_active(rb_spell_check_fr);
}

//-----------------------------------------------------------------------------
//--NEURAL-NETWORK-WINDOW
void window_nn_create()
{
    gtk_widget_destroy(window_nn);
    gtk_builder_add_from_file (builder, "gui/window_main.glade", NULL);
    gtk_builder_connect_signals(builder, NULL);
    window_nn = GTK_WIDGET(
            gtk_builder_get_object(builder, "window_nn"));
    gtk_widget_show(window_nn);
}

void on_btn_nn_close_clicked()
{
    gtk_widget_destroy(window_nn);
}

//-----------------------------------------------------------------------------
//--MAIN-BUTTONS
void on_btn_image_selection_clicked()
{
    window_file_selection_create();
}

void on_cb_advanced_toggled()
{
    if (window_advanced)
        gtk_widget_destroy(window_advanced);
    isactive_advanced = gtk_toggle_button_get_active(cb_advanced);
}

void on_btn_convert_clicked()
{
    //window_loading_create();
    //sleep(2);
    if (isactive_advanced)
    {
        window_advanced_create();
    }
    else
    {
        printf("start image convertion with normal settings\n");
        //convert();
    }
    //gtk_widget_destroy(window_loading);
}

void on_btn_text_save_clicked()
{
    window_file_save_create();
}

void on_btn_text_copy_clicked()
{
    const char *message = gchar_from_text_view(g_text_result);
    gtk_clipboard_set_text(gtk_clipboard_get(GDK_SELECTION_CLIPBOARD),
            message, strlen(message));
    gtk_clipboard_store(gtk_clipboard_get(GDK_SELECTION_CLIPBOARD));
}

//-----------------------------------------------------------------------------
//--TOPBAR-MENU
void on_menu_file_add_image_activate()
{
    window_file_selection_create();
}

void on_menu_file_save_text_activate()
{
    on_btn_text_save_clicked();
}

void on_menu_file_copy_text_activate()
{
    on_btn_text_copy_clicked();
}

void on_menu_file_quit_activate()
{
    gtk_main_quit();
}


void on_menu_view_best_fit_activate()
{
    zoom_bestfit = 1;
    zoom_largefit = 0;
    zoom_normal = 0;
    on_window_main_size_allocate();
}

void on_menu_view_large_fit_activate()
{
    zoom_bestfit = 0;
    zoom_largefit = 1;
    zoom_normal = 0;
    on_window_main_size_allocate();
}

void on_menu_view_normal_size_activate()
{
    zoom_bestfit = 0;
    zoom_largefit = 0;
    zoom_normal = 1;
    on_window_main_size_allocate();
}

void on_menu_view_zoom_in_activate()
{
    zoom_bestfit = 0;
    zoom_largefit = 0;
    zoom_normal = 0;

    GdkPixbuf *current = gtk_image_get_pixbuf(g_image_main);
    float r_image = (float)gdk_pixbuf_get_height(pixbuf)/
        gdk_pixbuf_get_width(pixbuf);
    int desired_width =  gdk_pixbuf_get_width(current) + 40;
    int desired_height = (int)(desired_width * r_image);
    gtk_image_set_from_pixbuf(g_image_main, gdk_pixbuf_scale_simple(pixbuf,
        desired_width, desired_height, GDK_INTERP_BILINEAR));

    gtk_widget_queue_resize(window_main);
}

void on_menu_view_zoom_out_activate()
{
    zoom_bestfit = 0;
    zoom_largefit = 0;
    zoom_normal = 0;

    GdkPixbuf *current = gtk_image_get_pixbuf(g_image_main);
    float r_image = (float)gdk_pixbuf_get_height(pixbuf)/
        gdk_pixbuf_get_width(pixbuf);
    int desired_width =  gdk_pixbuf_get_width(current) - 40;
    if (desired_width < 40)
        desired_width += 40;
    int desired_height = (int)(desired_width * r_image);
    gtk_image_set_from_pixbuf(g_image_main, gdk_pixbuf_scale_simple(pixbuf,
        desired_width, desired_height, GDK_INTERP_BILINEAR));

    gtk_widget_queue_resize(window_main);
}

void on_menu_tools_deskew_activate()
{
    //TODO
    gchar_to_text_view(g_text_result, "De-skew tool! (not implemented yet ...)\n");
}

void on_menu_tools_spellchecker_activate()
{
    GtkWidget *spell_checker_dialog = gspell_checker_dialog_new(
            GTK_WINDOW(window_main),
            gspell_navigator_text_view_new(g_text_result));
    gtk_widget_show(spell_checker_dialog);
}

void on_menu_tools_neural_network_activate()
{
    //TODO
    gchar_to_text_view(g_text_result, "Neural-network preferences!\n(not implemented yet ...)\n");
}

void on_menu_help_help_activate()
{
    int opened_url =
        g_app_info_launch_default_for_uri(
            "http://github.com/4pm-nomnom/OCR", NULL, NULL);
    if (!opened_url)
    {
        printf("failed to open the help page... open it directly :\n");
        printf("https://github.com/4pm-nomnom/OCR\n");
    }
}

void on_menu_help_about_activate()
{
    window_about_create();
}
