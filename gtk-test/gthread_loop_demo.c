#include <gtk/gtk.h>

gboolean thread1_end = FALSE;
gboolean thread2_end = FALSE;

gpointer thread1(gpointer data) {
    int i;
    for(i = 0; i < 10; i++) {
        g_print("Thread1: %s\n", data);
        g_usleep(1000000);
    }
    thread1_end = TRUE;
}

gpointer thread2(gpointer data) {
    int i;
    for(i = 0; i < 10; i++) {
        g_print("Thread2: %s\n", data);
        g_usleep(1000000);
    }
    thread2_end = TRUE;
}

gpointer checking_thread(gpointer mloop) {
    while(TRUE) {
        if(thread1_end && thread2_end) {
            g_main_loop_quit(mloop);
            break;
        }
        g_usleep(1000);
    }
}

int main(int argc, char *argv[]) {
    GMainLoop *mloop;
    
    if(!g_thread_supported()) {
        g_thread_init(NULL);
    }
    
    mloop = g_main_loop_new(NULL, FALSE);

    g_thread_create(thread1, "Running", FALSE, NULL);
    g_thread_create(thread2, "Going", FALSE, NULL);
    g_thread_create(checking_thread, mloop, FALSE, NULL);
    
    g_main_loop_run(mloop);
    
    return 0;
}

