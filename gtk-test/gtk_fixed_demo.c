#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button1, *button2, *button3;
    GtkWidget *fixed;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkFixed");
    gtk_window_set_default_size(GTK_WINDOW(window), 320, 200);

    button1 = gtk_button_new_with_label("Press 1");
    button2 = gtk_button_new_with_label("Press 2");
    button3 = gtk_button_new_with_label("Press 3");

    fixed = gtk_fixed_new();

    gtk_fixed_put(GTK_FIXED(fixed), button1, 10, 10);
    gtk_fixed_put(GTK_FIXED(fixed), button2, 50, 100);
    gtk_fixed_put(GTK_FIXED(fixed), button3, 200, 80);

    gtk_container_add(GTK_CONTAINER(window), fixed);

    g_signal_connect(GTK_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
