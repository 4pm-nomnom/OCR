#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h> // graphical user interfaces

GtkImage *g_image_main;
GtkWidget *image_box;
GdkPixbuf *pixbuf;
int zoom_bestfit = 1;
int zoom_largefit = 0;

int main(int argc, char *argv[])
{
    // --- GTK Window ------------------------------------------
    GtkBuilder *builder;
    GtkWidget *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "gui/image_scale.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    g_image_main = GTK_IMAGE(gtk_builder_get_object(builder, "image_main"));

    image_box = GTK_WIDGET(gtk_builder_get_object(builder, "image_box"));

    pixbuf = gdk_pixbuf_new_from_file("../samples/hey.png", NULL);
    gtk_image_set_from_pixbuf(g_image_main, pixbuf);

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
    else
        gtk_image_set_from_pixbuf(g_image_main, pixbuf);
}

void on_btn_bestfit_clicked()
{
    zoom_bestfit = 1;
    zoom_largefit = 0;
    on_window_main_size_allocate();
}

void on_btn_largefit_clicked()
{
    zoom_bestfit = 0;
    zoom_largefit = 1;
    on_window_main_size_allocate();
}

void on_btn_normal_size_clicked()
{
    zoom_bestfit = 0;
    zoom_largefit = 0;
    on_window_main_size_allocate();
}
