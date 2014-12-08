#include <gtk/gtk.h>

// 建立水平按鈕群組
GtkWidget* createHButtonBoxWithFrame() {
    int i;
    GtkWidget *hbuttonBox = gtk_hbutton_box_new();
    GtkWidget *gtkFrame = gtk_frame_new("GtkHButtonBox");
    gtk_container_add(GTK_CONTAINER(gtkFrame), hbuttonBox);

    gtk_button_box_set_layout(GTK_BUTTON_BOX(hbuttonBox),
                              GTK_BUTTONBOX_SPREAD);
    for(i = 0; i < 4; i++) {
        gtk_box_pack_start(GTK_BOX(hbuttonBox),
           gtk_button_new_with_label("HBtn"), TRUE, TRUE, 5);
    }

    return gtkFrame;
}

// 建立垂直按鈕群組
GtkWidget* createVButtonBoxWithFrame() {
    int i;
    GtkWidget *vbuttonBox = gtk_vbutton_box_new();
    GtkWidget *gtkFrame = gtk_frame_new("GtkVButtonBox");
    gtk_container_add(GTK_CONTAINER(gtkFrame), vbuttonBox);

    gtk_button_box_set_layout(GTK_BUTTON_BOX(vbuttonBox),
                              GTK_BUTTONBOX_START);
    for(i = 0; i < 4; i++) {
        gtk_box_pack_start(GTK_BOX(vbuttonBox),
           gtk_button_new_with_label("VBtn"), TRUE, TRUE, 5);
    }

    return gtkFrame;
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *gtkFrame1, *gtkFrame2;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkFrame & GtkButtonBox");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);

    // 最外層為GtkFrame，當中包括GtkVBox
    vbox = gtk_vbox_new(TRUE, 5);
    gtkFrame1 = gtk_frame_new("GtkVBox");
    gtk_container_add(GTK_CONTAINER(gtkFrame1), vbox);

    // 建立水平按鈕群組，加入GtkVBox
    gtk_box_pack_start(GTK_BOX(vbox), 
        createHButtonBoxWithFrame(), TRUE, TRUE, 5);

    // 建立GtkFrame，當中包括GtkHBox，加入GtkVBox中
    hbox = gtk_hbox_new(TRUE, 5);
    gtkFrame2 = gtk_frame_new("GtkHBox");
    gtk_container_add(GTK_CONTAINER(gtkFrame2), hbox);
    gtk_box_pack_start(GTK_BOX(vbox), gtkFrame2, TRUE, TRUE, 5);

    // 分別建立兩個垂直按鈕群組，加入GtkHBox中
    gtk_box_pack_start(GTK_BOX(hbox), 
        createVButtonBoxWithFrame(), TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), 
        createVButtonBoxWithFrame(), TRUE, TRUE, 5);

    gtk_container_add(GTK_CONTAINER(window), gtkFrame1);

    g_signal_connect(GTK_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}


