#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *nameEntry, *pwdEntry;
    GtkWidget *table;

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkEntry");

    nameEntry = gtk_entry_new();
    pwdEntry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(pwdEntry), FALSE);

    table = gtk_table_new(2, 2, TRUE);
    gtk_table_attach_defaults(GTK_TABLE(table), 
         gtk_label_new("Name"), 0, 1, 0, 1);   
    gtk_table_attach_defaults(GTK_TABLE(table), nameEntry, 1, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), 
         gtk_label_new("Password"), 0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), pwdEntry, 1, 2, 1, 2);

    gtk_container_add(GTK_CONTAINER(window), table);

    g_signal_connect(GTK_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
