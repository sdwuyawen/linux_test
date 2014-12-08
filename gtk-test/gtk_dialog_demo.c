#include <gtk/gtk.h>

void clicked_callback(GtkButton *button, GtkWindow *window) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("GtkDialog", window,
                            GTK_DIALOG_DESTROY_WITH_PARENT,
                            GTK_STOCK_OK, GTK_RESPONSE_OK,
                            NULL);

    GtkWidget *label = gtk_label_new(NULL);
    GtkWidget *image = gtk_image_new_from_stock(GTK_STOCK_DIALOG_INFO,
                                                GTK_ICON_SIZE_DIALOG);
    GtkWidget *hbox = gtk_hbox_new(FALSE, 5);

    gtk_dialog_set_has_separator(GTK_DIALOG(dialog), FALSE);
    gtk_label_set_markup(GTK_LABEL(label), 
      "<b>良葛格學習筆記</b>\nhttp://caterpillar.onlyfun.net");

    gtk_container_set_border_width(GTK_CONTAINER (hbox), 10);
    gtk_box_pack_start_defaults(GTK_BOX(hbox), image);
    gtk_box_pack_start_defaults(GTK_BOX(hbox), label);

    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), hbox);

    g_signal_connect(G_OBJECT(dialog), "response",
                     G_CALLBACK(gtk_widget_destroy), NULL);

    gtk_widget_show_all(dialog);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button;

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkDialog");

    button = gtk_button_new_with_label("按我");

    gtk_container_add(GTK_CONTAINER(window), button);

    g_signal_connect(GTK_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(GTK_OBJECT(button), "clicked",
                     G_CALLBACK(clicked_callback), window);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}


