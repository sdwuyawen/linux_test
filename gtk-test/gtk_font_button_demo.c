#include <gtk/gtk.h>

void font_changed(GtkFontButton *button, gpointer label) {
    const gchar *fontName = gtk_font_button_get_font_name(button);
    PangoFontDescription *fontDesc = 
          pango_font_description_from_string(fontName);
    gtk_widget_modify_font(GTK_WIDGET(label), fontDesc);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *label;
    GtkWidget *button;
    GtkWidget *box;
    PangoFontDescription *fontDesc;

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkFontButton");

    label = gtk_label_new("Hello World!");
    fontDesc = pango_font_description_from_string("Courier Pitch Bold 12");
    gtk_widget_modify_font(label, fontDesc);

    button = gtk_font_button_new_with_font("Courier Pitch Bold 12");
    box = gtk_hbox_new(TRUE, 5);
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 5);

    gtk_container_add(GTK_CONTAINER(window), box);

    g_signal_connect(GTK_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(GTK_OBJECT(button), "font_set",
                     G_CALLBACK(font_changed), label);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
