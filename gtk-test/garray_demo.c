#include <glib.h>

int main(int argc, char *argv[]) {
    GArray *garray;
    gint i;

    garray = g_array_new(FALSE, FALSE, sizeof(gint));
      
    for (i = 0; i < 5; i++) {
        g_array_append_val(garray, i);
    }

    for (i = 0; i < 5; i++) {
        g_printf("%d\n", g_array_index(garray, gint, i));
    }

    g_array_free (garray, TRUE);
     
    return 0;
}
