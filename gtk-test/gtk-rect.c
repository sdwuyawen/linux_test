/**
 *	@filename :
 * 		gtk-test.c
 * 
 * 	@note :
 * 
 **/
#include <gtk/gtk.h>
#include <string.h>

#define TEST_RECTS_MAX		16
#define TEST_BUFFER_SIZE	256

#define TEST_WIN_W			480
#define TEST_WIN_H			320

static int g_rect_num;
static GdkRectangle g_src_rects[TEST_RECTS_MAX];
static GdkGC *g_gcs[TEST_RECTS_MAX];

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
    GdkColor color;
    int i;
    GdkGC *gc;
    
    if (g_rect_num == 0) {
		return FALSE;
	}
    
    memset(&color, 0, sizeof(color));   
     	
    gc = gdk_gc_new( da->window );    
	gdk_gc_set_rgb_fg_color(gc, &color);
	gdk_draw_rectangle( da->window,
		   gc,
		   TRUE,//fill
		   0, 0, TEST_WIN_W, TEST_WIN_H);  
    
    i = g_rect_num - 1;
    do {
	    color.red = (i==0)?65535:0;
		color.green = (i==1)?65535:0;
		color.blue = (i==2)?65535:0;		
		g_gcs[i] = gdk_gc_new( da->window );    
		gdk_gc_set_rgb_fg_color(g_gcs[i], &color);
		gdk_gc_set_function(g_gcs[i], GDK_XOR);
		gdk_draw_rectangle( da->window,
               g_gcs[i],
               TRUE,//fill
               //FALSE,
               g_src_rects[i].x, g_src_rects[i].y, g_src_rects[i].width, g_src_rects[i].height);  
	} while (i-- > 0);

    for (i=0; i<g_rect_num; i++) {	
		g_object_unref( G_OBJECT(g_gcs[i]) );
	}
	g_object_unref( G_OBJECT(gc) );
  
    return TRUE;
}

int main( int argc, char *argv[] )
{
	GtkWidget *window;
	GtkWidget *button;
	GtkWidget *da; //drawing area
	FILE *pf = NULL;
	char buffer[TEST_BUFFER_SIZE];
	
	if (argc != 2) {
		printf("%s rect.lst", argv[0]);
		return -1;
	}
	
	pf = fopen(argv[1], "r");
	if (pf == NULL) {
		printf("can not open file %s\n", argv[1]);
		return -1;
	}
	
	memset(g_src_rects, 0, sizeof(g_src_rects));
	g_rect_num = 0;
	while (fgets(buffer, TEST_BUFFER_SIZE, pf)) {
		if (buffer[0] == '#') {
			continue;
		}
		
		g_src_rects[g_rect_num].x = atoi(strtok(buffer, ","));
		g_src_rects[g_rect_num].y = atoi(strtok(NULL, ","));
		g_src_rects[g_rect_num].width = atoi(strtok(NULL, ","));
		g_src_rects[g_rect_num].height = atoi(strtok(NULL, ","));
		
		g_rect_num++;
		if (g_rect_num == TEST_RECTS_MAX) {
			break;
		}
	}	
	fclose(pf);
		
	gtk_init (&argc, &argv);
	
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), TEST_WIN_W, TEST_WIN_H);
	gtk_signal_connect(GTK_OBJECT(window), "delete_event", GTK_SIGNAL_FUNC(delete_event), NULL);
	gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(destroy), NULL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	
	da = gtk_drawing_area_new ();
    /* set a minimum size */
    gtk_widget_set_size_request( da, TEST_WIN_W, TEST_WIN_H );
    gtk_container_add( GTK_CONTAINER(window), da );
    g_signal_connect( da, "expose_event",
             G_CALLBACK(my_expose), NULL );
    gtk_widget_show_all( window ); //simply show all widgets
    
	gtk_main();
	
	return(0);
}
