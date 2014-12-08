#include <gtk/gtk.h>

void file_changed(GtkFileChooserButton *button, GtkTextView *textView) {
    gchar *filename; 
    gchar *content;
    gsize bytes;
    GError *error = NULL;
    GtkTextBuffer *buffer;

    filename  =  gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(button));

    g_file_get_contents(filename, &content, &bytes, &error);
    if (error != NULL) {
        g_printf(error->message);
        g_clear_error(&error);
    }
  
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (textView));
    gtk_text_buffer_set_text (buffer, content, -1);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *scrolledWindow;
    GtkWidget *textView;
    GtkWidget *button;
    GtkWidget *vbox;
    PangoFontDescription *fontDesc;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkTextView");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 50);

    scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    textView = gtk_text_view_new();
    fontDesc = pango_font_description_from_string("Courier Pitch Bold 12");
    gtk_widget_modify_font(textView, fontDesc);

    gtk_container_add(GTK_CONTAINER(scrolledWindow), textView);

    button = gtk_file_chooser_button_new(
                 "選取檔案", GTK_FILE_CHOOSER_ACTION_OPEN);

    vbox = gtk_vbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), scrolledWindow, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    g_signal_connect(GTK_OBJECT(button), "selection_changed",
                     G_CALLBACK(file_changed), textView);
    g_signal_connect(GTK_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
