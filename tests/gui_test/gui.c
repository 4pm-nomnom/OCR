#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h> // graphical user interfaces

GtkImage *g_image_main;
GtkFileChooser *g_btn_file_chooser;
GtkWidget *g_text_result;

int main(int argc, char *argv[])
{
    // --- GTK Window ------------------------------------------
    GtkBuilder *builder;
    GtkWidget *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "gui/window_main.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    // get pointers to the labels
    g_image_main = GTK_IMAGE(gtk_builder_get_object(builder, "image_main"));
    g_btn_file_chooser = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "btn_file_chooser"));
    g_text_result = GTK_WIDGET(gtk_builder_get_object(builder, "text_result"));

    g_object_unref(builder);

    gtk_widget_show(window);
    gtk_main();

    return EXIT_SUCCESS;
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}

//----------------------MENU------------------------------
void on_menu_file_add_image_activate()
{
    
    //TODO
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
    //TODO
    
}


void on_btn_file_chooser_file_set()
{
    gchar *image_path = gtk_file_chooser_get_filename(g_btn_file_chooser);
    gtk_image_set_from_file(g_image_main, image_path);
}


void on_btn_advanced_toggled()
{
    //TODO change button variable state
    
}

void on_btn_convert_activate()
{
    //TODO CONVERTION OCR WOW
    
}


void on_btn_text_save_activate()
{
    //TODO save current text contained in text_result
    
}

void on_btn_text_copy_activate()
{
    //TODO copy to clipboard content of text_result
    
}
