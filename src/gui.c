#include <stdio.h>
#include <stdlib.h>
// #include "SDL/SDL.h" //included in image.h
// #include "SDL/SDL_image.h" //included in image.h
#include <gtk/gtk.h> // graphical user interfaces
#include "glib/gprintf.h"
#include <gspell/gspell.h>

#include "image.h"
#include "preprocessing.h"
#include "segmentation.h"
#include "nn/neural_net.h"

GtkImage *g_image_main;
gchar *currentImage;
GtkWidget *image_box;
GdkPixbuf *pixbuf;
GtkWidget *g_lbl_image_name;
GtkFileChooser *g_file_selection;
GtkFileChooser *g_file_save;
GtkTextView *g_text_result;
GspellNavigator *g_spell_navigator;
GtkImage *image_show_steps;
GtkWidget *show_steps_image_box;

GtkToggleButton *cb_advanced;
GtkToggleButton *cb_show_pre_processing;
GtkToggleButton *cb_show_segmentation;
GtkToggleButton *rb_spell_check_en;
GtkToggleButton *rb_spell_check_fr;
GtkToggleButton *rb_spell_check_disable;
GtkToggleButton *rb_show_original;
GtkToggleButton *rb_show_grayscale;
GtkToggleButton *rb_show_binarized;
GtkToggleButton *rb_show_segmentation;

GtkWidget *window_main;
GtkWidget *window_about;
GtkWidget *window_file_selection;
GtkWidget *window_file_save;
GtkWidget *window_advanced;
GtkWidget *window_show;
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

    //zoom key
    gtk_widget_add_events(image_box, GDK_KEY_PRESS_MASK);

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
    //if not created
    char *imgPath = currentImage;

    //--- Load image --------------------------------------------
    SDL_Surface* image_surface;
    init_sdl();

    image_surface = load_image(imgPath);
    size_t img_width = image_surface->w;
    size_t img_height = image_surface->h;

    Surface_save_image(image_surface, "tmp/original.bmp");
    
    size_t *img_bin_matrix = matrix_from_image_preprocessing(image_surface);

    //matrix_print(img_bin_matrix, img_height, img_width);

    /************************************************************
     *                   Character Detection                     *
     * In this section the layout will be analysed. This means   *
     * the segmentation of blocks/lines/words/characters.        *
     *************************************************************/

    //--- Get Lines ---------------------------------------------
    TextLine *textLines = calloc(MAX_NUMBER_OF_LINES, sizeof(TextLine));
    size_t nbTextLines = TextLines_find(textLines,
            img_bin_matrix, img_height, img_width);
    TextLines_show(textLines, nbTextLines);

    //--- Get Characters ----------------------------------------
    for(size_t i=0; i < nbTextLines; i++)
    {
        TextLine *currentLine = &textLines[i];

        currentLine->Characters =
            calloc(MAX_NUMBER_OF_CHARACTERS, sizeof(Character));
        get_characters(currentLine, img_bin_matrix, img_width, img_height);

        printf("Line[%zu] -> nbCharacters detected : %zu\n",
                i, currentLine->nbCharacters);
    }

    // save images of the segmentation
    Surface_draw_textLines(image_surface, textLines, nbTextLines);
    Surface_save_image(image_surface, "tmp/segmentation.bmp");

    /************************************************************
     *                  Character Recognition                    *
     * Convert the input matrix (representing a character) to an *
     * ASCII                                                     *
     *************************************************************/
    gchar *recognized_text = "";
    for(size_t i=0; i<nbTextLines; i++)
    {
        for(size_t c=0; c<textLines[i].nbCharacters; c++)
        {
            //size_t *char_matrix = textLines[i].Characters[c].matrix;
            // char result_char = nn_character_recognition(char_matrix);
            gchar *result_char = "a";
            recognized_text = g_strconcat(recognized_text, result_char, NULL);

            //is there a space after this char ?
            if (c+1<textLines[i].nbCharacters &&
                textLines[i].Characters[c+1].LeftBound -
                textLines[i].Characters[c].RightBound >
                textLines[i].averageSpaceWidth*1.2)
                recognized_text = g_strconcat(recognized_text, " ", NULL);

            free(textLines[i].Characters[c].matrix);
        }
        recognized_text = g_strconcat(recognized_text, "\n", NULL);
        free(textLines[i].Characters);
    }
    //textlines
    free(textLines);
    gchar_to_text_view(g_text_result, recognized_text);
    g_free(recognized_text);
    
    /************************************************************
     *                     Post-processing                       *
     * Possible checking of the words by using a dictionnary or  *
     * a lexicon - a list of words that are allowed to occur     *
     *************************************************************/

    /************************************************************
     *                        Free Memory                        *
     *************************************************************/
    free(img_bin_matrix);

    // Free the image surface.
    SDL_FreeSurface(image_surface);

    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
//--MAIN-FUNCTIONS
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
}

void on_btn_file_selection_cancel_clicked()
{
    gtk_widget_destroy(window_file_selection);
}

gboolean file_isimage(gchar *file_path)
{
    return (
        g_str_has_suffix(file_path, ".jpg") ||
        g_str_has_suffix(file_path, ".png") ||
        g_str_has_suffix(file_path, ".pdf") ||
        g_str_has_suffix(file_path, ".bmp")
        );
}

void on_btn_file_selection_open_clicked()
{

    //file full path
    gchar *file_path = gtk_file_chooser_get_filename(g_file_selection);

    if (file_isimage(file_path))
    {
        zoom_bestfit = 1;
        zoom_largefit = 0;
        zoom_normal = 0;
        if (g_str_has_suffix(file_path, ".pdf"))
        {
            int success = system(g_strconcat(
                "pdftocairo -l 5 -png ", file_path, " tmp/pdf",
                ";convert -append tmp/pdf-*.png tmp/pdf_view.png", NULL));
            if (success != -1)
            {
                pixbuf = gdk_pixbuf_new_from_file("tmp/pdf_view.png", NULL);
                currentImage = g_strconcat ("tmp/pdf_view.png", NULL);
            }
            else
            {
                gchar_to_text_view(g_text_result,
                    "Failed to convert your pdf, sorry for that !\n");
            }
        }
        else
        {
            pixbuf = gdk_pixbuf_new_from_file(file_path, NULL);
            currentImage = g_strconcat (file_path, NULL);
        }
        on_window_main_size_allocate();

        gsize maxlength = 12;
        gchar *image_name = g_strreverse(file_path); //path is reversed too care
        image_name = g_strndup(file_path, maxlength);
        image_name = g_strreverse(image_name);
        if (image_name)
        {
            gchar *dots = "...";
            image_name = g_strconcat(dots, image_name, NULL);
        }
        gtk_label_set_text(GTK_LABEL(g_lbl_image_name), image_name);

    }
    gtk_widget_queue_resize(window_main);
    gtk_widget_destroy(window_file_selection);
    g_free(file_path);
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
    gchar *filename;
    filename = gtk_file_chooser_get_filename(g_file_save);
    FILE *fs;
    fs = fopen(filename, "w");
    gchar *result_txt = gchar_from_text_view(g_text_result);
    fprintf(fs, "%s", result_txt);
    g_free(filename);
    g_free(result_txt);
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
//--SHOW-WINDOW
void change_image_preview(gchar *imagePath);
void window_show_create()
{
    if (window_show)
        gtk_widget_destroy(window_show);

    gtk_builder_add_from_file (builder, "gui/window_main.glade", NULL);
    gtk_builder_connect_signals(builder, NULL);
    window_show = GTK_WIDGET(
            gtk_builder_get_object(builder, "window_show"));

    image_show_steps = GTK_IMAGE(
            gtk_builder_get_object(builder, "image_show_steps"));
    show_steps_image_box = GTK_WIDGET(
            gtk_builder_get_object(builder, "show_steps_image_box"));

    rb_show_original = GTK_TOGGLE_BUTTON(
            gtk_builder_get_object(builder, "rb_btn_show_original"));
    rb_show_grayscale = GTK_TOGGLE_BUTTON(
            gtk_builder_get_object(builder, "rb_btn_show_grayscale"));
    rb_show_binarized = GTK_TOGGLE_BUTTON(
            gtk_builder_get_object(builder, "rb_btn_show_binarized"));
    rb_show_segmentation = GTK_TOGGLE_BUTTON(
            gtk_builder_get_object(builder, "rb_btn_show_segmentation"));

    gtk_toggle_button_set_active (rb_show_original, TRUE);
    gtk_toggle_button_set_active (rb_show_grayscale, FALSE);
    gtk_toggle_button_set_active (rb_show_binarized, FALSE);
    gtk_toggle_button_set_active (rb_show_segmentation, FALSE);

    gtk_widget_show(window_show);
    change_image_preview("tmp/original.bmp");
}

void on_btn_show_steps_quit_clicked()
{
    gtk_widget_destroy(window_show);
}

void change_image_preview(gchar *imagePath)
{
    GdkPixbuf *pixbuf_show = gdk_pixbuf_new_from_file(imagePath, NULL);
    GtkAllocation allocation;
    gtk_widget_get_allocation(
        show_steps_image_box,
        &allocation);
    int desired_width = allocation.width;
    int desired_height = allocation.height;
    float r_box = (float)desired_height/desired_width;
    float r_image = (float)gdk_pixbuf_get_height(pixbuf)/
        gdk_pixbuf_get_width(pixbuf_show);

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
    gtk_image_set_from_pixbuf(image_show_steps,
        gdk_pixbuf_scale_simple(pixbuf_show,
            desired_width, desired_height, GDK_INTERP_BILINEAR));
    gtk_widget_queue_resize(window_show);
}

void on_rb_btn_show_toggled()
{
    if (gtk_toggle_button_get_active(rb_show_original))
        change_image_preview("tmp/original.bmp");
    if (gtk_toggle_button_get_active(rb_show_grayscale))
        change_image_preview("tmp/grayscale.bmp");
    if (gtk_toggle_button_get_active(rb_show_binarized))
        change_image_preview("tmp/binarized.bmp");
    if (gtk_toggle_button_get_active(rb_show_segmentation))
        change_image_preview("tmp/segmentation.bmp");
}

//-----------------------------------------------------------------------------
//--ADVANCED-CONVERT-WINDOW
void window_advanced_create()
{
    if (window_advanced)
        gtk_widget_destroy(window_advanced);

    isactive_show_preprocessing = FALSE;
    isactive_show_segmentation = FALSE;

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
    convert();
    if (isactive_show_preprocessing || isactive_show_segmentation)
        window_show_create();
}

void on_cb_show_pre_processing_toggled()
{
    isactive_show_preprocessing =
        gtk_toggle_button_get_active(cb_show_pre_processing);
}
void on_cb_show_segmentation_toggled()
{
    isactive_show_segmentation =
        gtk_toggle_button_get_active(cb_show_segmentation);
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
    if (currentImage)
    {
        if (isactive_advanced)
        {
            window_advanced_create();
        }
        else
        {
            printf("start image convertion with normal settings\n");
            convert();
        }
    }
    else
        gchar_to_text_view(g_text_result,
            "You need to select an Image to convert it !\n");
}

void on_btn_text_save_clicked()
{
    window_file_save_create();
}

void on_btn_text_copy_clicked()
{
    const gchar *message = gchar_from_text_view(g_text_result);
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
    gchar_to_text_view(g_text_result,
        "De-skew tool! (not implemented yet ...)\n");
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
    Neural_network();
    gchar_to_text_view(g_text_result, "Neural-network overview!\n");
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

gboolean on_image_main_key_press_event(GtkWidget *widget, GdkEventKey *event,
    gpointer data)
{
    (void)widget;
    (void)data;
    if (event->keyval == GDK_KEY_plus){
        on_menu_view_zoom_in_activate();
        return TRUE;
    }
    else if (event->keyval == GDK_KEY_minus){
        on_menu_view_zoom_out_activate();
        return TRUE;
    }
    return FALSE;
}
