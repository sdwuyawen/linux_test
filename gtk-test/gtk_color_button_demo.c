#include <gtk/gtk.h>

void color_changed(GtkColorButton *button, gpointer data) {
    GdkColor color;
    gtk_color_button_get_color(button, &color);
    gtk_widget_modify_fg(GTK_WIDGET(data), GTK_STATE_NORMAL, &color); //change foreground color
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *label;
    GtkWidget *button;
    GtkWidget *box;
    GdkColor color;

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkColorButton");

    gdk_color_parse("#FF0000", &color);	//red to color
    label = gtk_label_new("Color me");
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);	//change foreground color
    button = gtk_color_button_new_with_color(&color);

    box = gtk_hbox_new(TRUE, 5);
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 5);

    gtk_container_add(GTK_CONTAINER(window), box);

    g_signal_connect(GTK_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(GTK_OBJECT(button), "color_set",
                     G_CALLBACK(color_changed), label);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
