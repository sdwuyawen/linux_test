#include <gtk/gtk.h>

void file_changed(GtkFileChooserButton *button, GtkLabel *label) {
    gchar *file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(button));
    gtk_label_set_text(label, file);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *label;
    GtkWidget *button;
    GtkFileFilter *filter1, *filter2;
    GtkWidget *box;
    const gchar *filename = "/home/caterpillar/workspace/caterpillar.gif";

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkFileChooserButton");

    button = gtk_file_chooser_button_new(
                  "選取檔案", GTK_FILE_CHOOSER_ACTION_OPEN);
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(button), filename);

    filter1 = gtk_file_filter_new();
    filter2 = gtk_file_filter_new();
    gtk_file_filter_set_name(filter1, "Image Files");
    gtk_file_filter_set_name(filter2, "All Files");
    gtk_file_filter_add_pattern(filter1, "*.png");
    gtk_file_filter_add_pattern(filter1, "*.gif");
    gtk_file_filter_add_pattern(filter1, "*.jpg");
    gtk_file_filter_add_pattern(filter2, "*");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(button), filter1);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(button), filter2);

    label = gtk_label_new(filename);

    box = gtk_vbox_new(TRUE, 5);
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 5);

    gtk_container_add(GTK_CONTAINER(window), box);

    g_signal_connect(GTK_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(GTK_OBJECT(button), "selection_changed",
                     G_CALLBACK(file_changed), label);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
