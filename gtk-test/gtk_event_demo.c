/**
 * 
 * 	@note :
 * 	  事件的处理顺序：
		按鈕按下 --> 發出 GDK_BUTTON_PRESS --> GDK 預設處理函式
                 --> 發出 button_press_event Signal --> GTK 預設處理函式
         --> 發出 clicked Signal --> GTK 預設處理函式


 * 	  如果button_press_callback返回TRUE, 那么就不会继续 GTK預設處理函式
 * 
 **/
#include <gtk/gtk.h>

#define _DEMO_BTN_PRESS_ONLY

//GTK Signal處理函式
void button_clicked_callback(GtkWidget *button, gpointer data) {
    g_print("clicked處理函式\n");
}

//GDK事件處理函式
#ifdef _DEMO_BTN_PRESS_ONLY
gboolean button_press_callback(
           GtkWidget *button, GdkEventButton *event, gpointer data) {

	g_print("button_press_event處理函式(%d, %d)\n", 
			(gint) event->x, (gint) event->y);

	//return TRUE;
    return FALSE;
}
#else
gboolean button_press_callback(
           GtkWidget *button, GdkEvent *event, gpointer data) {
    GdkEventType type = event->type;

    if(type == GDK_BUTTON_PRESS) {
        g_print("button_press_event處理函式(%d, %d)\n", 
                (gint) event->button.x, (gint) event->button.y);
    }

	//return TRUE;
    return FALSE;
}
#endif

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button;

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "哈囉！GTK+！");

    button = gtk_button_new_with_label("按我");
    gtk_container_add(GTK_CONTAINER(window), button);

#ifdef _DEMO_BTN_PRESS_ONLY
    g_signal_connect(GTK_OBJECT(button), "button_press_event",
                     G_CALLBACK(button_press_callback), NULL);
#else                     
    g_signal_connect(GTK_OBJECT(button), "event",
                     G_CALLBACK(button_press_callback), NULL);
#endif                     
    g_signal_connect(GTK_OBJECT(button), "clicked",
                     G_CALLBACK(button_clicked_callback), NULL);
    g_signal_connect(GTK_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show(window);
    gtk_widget_show(button);

    gtk_main();

    return 0;
}
