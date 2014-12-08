#include <gtk/gtk.h>

void value_changed_callback(GtkSpinButton *spinButton, gpointer data) {
    gint value = gtk_spin_button_get_value_as_int(spinButton);
    GString *text = g_string_new("");
    g_string_sprintf(text, "%d", value);
    gtk_label_set_text(GTK_LABEL(data), text->str);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *spinButton;
    GtkWidget *label;
    GtkWidget *vbox;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkVBox");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 50);

    spinButton = gtk_spin_button_new_with_range(0.0, 100.0, 1.0);
    label = gtk_label_new("0");
    vbox = gtk_vbox_new(TRUE, 5);

    gtk_box_pack_start(GTK_BOX(vbox), spinButton, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    g_signal_connect(GTK_OBJECT(spinButton), "value_changed",
                     G_CALLBACK(value_changed_callback), label);
    g_signal_connect(GTK_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
