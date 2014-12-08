#include <gtk/gtk.h>

static GtkWidget *check1, *check2;

void toggled_callback(GtkToggleButton *toggleBtn, gpointer data) {    
	if (GTK_OBJECT(check1) == GTK_OBJECT(toggleBtn)) {
		printf("check1 clicked!\n");
	}
	
    printf("%s:%d\n", gtk_button_get_label(GTK_BUTTON(toggleBtn)), gtk_toggle_button_get_active(toggleBtn));      
}


GtkWidget* checkButtonsNew() {
    GtkWidget *vbox;

    check1 = gtk_check_button_new_with_label("Java is good!");
	g_signal_connect(GTK_OBJECT(check1), "toggled",
                     G_CALLBACK(toggled_callback), NULL);
                     
    check2 = gtk_check_button_new_with_label("C++ is good!");
	g_signal_connect(GTK_OBJECT(check2), "toggled",
                     G_CALLBACK(toggled_callback), NULL);
	
    vbox = gtk_vbox_new(TRUE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), check1, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), check2, TRUE, TRUE, 5);

    return vbox;
}

GtkWidget* radioButtonsNew() {
	GtkWidget *radio1, *radio2, *radio3;
    GtkWidget *vbox;
    
    radio1 = gtk_radio_button_new_with_label(NULL, "Linux");
	g_signal_connect(GTK_OBJECT(radio1), "toggled",
                     G_CALLBACK(toggled_callback), NULL);
    
    radio2 = gtk_radio_button_new_with_label_from_widget(
                 GTK_RADIO_BUTTON(radio1) , "Windows");
	g_signal_connect(GTK_OBJECT(radio2), "toggled",
                     G_CALLBACK(toggled_callback), NULL);
                 
    radio3 = gtk_radio_button_new_with_label_from_widget(
                 GTK_RADIO_BUTTON(radio1) , "Mac");
	g_signal_connect(GTK_OBJECT(radio3), "toggled",
                     G_CALLBACK(toggled_callback), NULL);

    vbox = gtk_vbox_new(TRUE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), radio1, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), radio2, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), radio3, TRUE, TRUE, 5);

    return vbox;
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *frame;

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkCheckButton & GtkRadioButton");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 50);

    vbox = gtk_vbox_new(FALSE, 5);

    gtk_box_pack_start(GTK_BOX(vbox), checkButtonsNew(), TRUE, TRUE, 5);

    frame = gtk_frame_new("Favorite OS");
    gtk_container_add(GTK_CONTAINER(frame), radioButtonsNew());

    gtk_box_pack_start(GTK_BOX(vbox), frame, TRUE, TRUE, 5);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    g_signal_connect(GTK_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
