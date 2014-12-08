#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *table;
    GtkWidget *label;

    const char *text[] = {"One", "Two", "Three",
                          "Four", "Five", "Six",
                          "Seven", "Eight", "Nine"};
    int i, j, k;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkTable");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 150);

    table = gtk_table_new(3, 3, TRUE);
    for(i = 0, k = 0; i < 3; i++, k = k + 3) {
        for(j = 0; j < 3; j++) {
            label = gtk_label_new(text[k + j]);
            gtk_table_attach_defaults(
                 GTK_TABLE(table), label, j, j + 1, i, i + 1);
        }
    }
    
    gtk_container_add(GTK_CONTAINER(window), table);

    g_signal_connect(GTK_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
