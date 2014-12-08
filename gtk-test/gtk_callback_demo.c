/**
 * 
 * 	@note :
  	1).g_signal_connect_swapped(GTK_OBJECT(button), "clicked",
                             G_CALLBACK(gtk_widget_destroy), window);
	相当于
	void button_clicked(GtkWidget *button, gpointer data) {
		gtk_widget_destroy((GtkWidget*) data);
	}	
	
	{
		...
		g_signal_connect(GTK_OBJECT(button), "clicked",
                     G_CALLBACK(button_clicked), NULL);	
        ...             
	} 
	
	2). 自动发出事件
	void g_signal_emit_by_name(gpointer instance,
							 const gchar *detailed_signal,
							 ...);
 **/ 
#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button;

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "哈囉！GTK+！");

    button = gtk_button_new_with_label("按我");
    gtk_container_add(GTK_CONTAINER(window), button);

    g_signal_connect(GTK_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect_swapped(GTK_OBJECT(button), "clicked",
                             G_CALLBACK(gtk_widget_destroy), window);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
