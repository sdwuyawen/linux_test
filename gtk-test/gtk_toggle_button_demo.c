#include <gtk/gtk.h>

void toggled_callback(GtkToggleButton *toggleBtn, gpointer data) {
    if (gtk_toggle_button_get_active(toggleBtn)) {
        gtk_button_set_label(GTK_BUTTON(toggleBtn), "停駐");
    } else {
        gtk_button_set_label(GTK_BUTTON(toggleBtn), "未停駐");
    }
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *toggleBtn;
    GtkWidget *hbox;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkToggleButton");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 50);

    toggleBtn = gtk_toggle_button_new_with_label("停駐");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggleBtn), TRUE);

    gtk_container_add(GTK_CONTAINER(window), toggleBtn);

    g_signal_connect(GTK_OBJECT(toggleBtn), "toggled",
                     G_CALLBACK(toggled_callback), NULL);
    g_signal_connect(GTK_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
