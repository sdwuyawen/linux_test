/**
 *	@filename :
 * 		gtk-test.c
 * 
 * 	@note :
 * 
 **/
#include <gtk/gtk.h>

static gboolean screen_shot(const char *file) 
{
	GdkScreen *screen = gdk_screen_get_default();
	//GdkWindow * rootWindow =gdk_screen_get_root_window(screen);
	GdkWindow * root_window = gdk_get_default_root_window (); //这个和上面谁人函数一样的
	if (!root_window ) {  
		return FALSE;
	}

	int w = gdk_screen_get_width(screen);
	int h = gdk_screen_get_height(screen);

	GdkPixbuf * pixbuf = gdk_pixbuf_get_from_drawable(NULL, root_window, NULL,
					   0, 0, 0, 0, w, h); // 抓图
	gdk_pixbuf_save(pixbuf, file, "jpeg", NULL, "quality", "100",
								NULL);      // 将图片存为jpg技俩		
								
	return TRUE;								
}

static void hello(GtkWidget *widget,gpointer data)
{
	g_print("Hello Ubuntu!\n");
	//screen_shot("test.jpg");
}

static gint delete_event(GtkWidget *widget,GdkEvent *event,gpointer data)
{
	g_print ("delete event occurred\n");
	
	//return TRUE;		//not call destroy()
	return FALSE;		//call destroy()
}

static void destroy(GtkWidget *widget,gpointer data)
{
	gtk_main_quit();
}

static gboolean
my_expose( GtkWidget      *da,
     GdkEventExpose *event,
     gpointer        data )
{
    GdkGC *gc1, *gc2, *gc;  /* gc1为矩形，gc2为文字 */
    GdkColor color;
    gc1 = gdk_gc_new( da->window );
    color.red = 30000;
    color.green = 0;
    color.blue = 30000;
    gdk_gc_set_rgb_fg_color(gc1, &color );
    gc2 = gdk_gc_new( da->window );
    color.red = 65535;
    color.green = 0;
    color.blue = 65535;
    gdk_gc_set_rgb_fg_color( gc2, &color );
    gc = gc1;
    gdk_draw_rectangle( da->window,
               gc,
               TRUE,//fill or not
               5, 5, 100, 100 );  /*（分别是越大则向右移动、越大则向下移、越大则向右扩大、越大则向下扩大）*/
    gc = gc2;
    gdk_draw_arc( da->window,
         gc,
         FALSE, //don't fill
         105, 5, 100, 100, 0, 60 * 64 );  /*（分别是越大则向右移动、越大则向下移、越大则向右扩大、越大则向下扩大、0不知道代表什么、弧线弧度）*/
 
    PangoLayout *layout =
        gtk_widget_create_pango_layout( da, "I'm a demon\nfrom hell.\nhahaha." );
    PangoFontDescription *fontdesc =
        pango_font_description_from_string( "Luxi Mono 12" );
    pango_layout_set_font_description( layout, fontdesc );
    gdk_draw_layout( da->window,
                     gc1,
                     5, 105, layout );
    pango_font_description_free( fontdesc );
    g_object_unref( layout );
 
    g_object_unref( G_OBJECT(gc1) );
    g_object_unref( G_OBJECT(gc2) );
  
    return TRUE;
}

int main( int argc, char *argv[] )
{
	GtkWidget *window;
	GtkWidget *button;
	GtkWidget *da; //drawing area
	
	gtk_init (&argc, &argv);
	
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), 480, 320);
	gtk_signal_connect(GTK_OBJECT(window), "delete_event", GTK_SIGNAL_FUNC(delete_event), NULL);
	gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(destroy), NULL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
#if 0	
	button = gtk_button_new_with_label("Hello Ubuntu!");	
	gtk_signal_connect(GTK_OBJECT(button), "clicked", GTK_SIGNAL_FUNC(hello), NULL);
	gtk_signal_connect_object(GTK_OBJECT(button), "clicked", GTK_SIGNAL_FUNC(gtk_widget_destroy), GTK_OBJECT(window));
	gtk_container_add(GTK_CONTAINER(window), button);
#else	
	da = gtk_drawing_area_new ();
    /* set a minimum size */
    gtk_widget_set_size_request( da, 300, 300 );
    gtk_container_add( GTK_CONTAINER(window), da );
    g_signal_connect( da, "expose_event",
             G_CALLBACK(my_expose), NULL );
#endif	
    gtk_widget_show_all( window ); //simply show all widgets
    
	gtk_main();
	
	return(0);
}
