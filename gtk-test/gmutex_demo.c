#include <glib.h>

GMutex *mutex = NULL;

struct _User {
    GString *name;
    GString *id;
    glong count;    
};
typedef struct _User User;

void user_set_name_id(User *user, GString *name, GString *id) {
    if(g_mutex_trylock(mutex)) {
        user->name = name;
        user->id = id;
        if(!user_check_name_id(user)) {
            g_print("%d: illegal name or id....\n", user->count);
        }
        g_print("name=%s, id=%s\n", name->str, id->str);
        user->count++;
        g_mutex_unlock(mutex);
    }
    else {
        g_usleep(1000);
    }
}

gboolean user_check_name_id(User *user) {    
    return user->name->str[0] == user->id->str[0];
}

gpointer thread1(gpointer user) {
    GString *name = g_string_new("Justin Lin");
    GString *id = g_string_new("J.L.");
    while(TRUE) {
        user_set_name_id(user, name, id);
    }
}

gpointer thread2(gpointer user) {
    GString *name = g_string_new("Shang Hwang");
    GString *id = g_string_new("S.H.");
    while(TRUE) {
        user_set_name_id(user, name, id);
    }
}

int main(int argc, char *argv[]) {
    GMainLoop *mloop;
    
    if(!g_thread_supported()) {
        g_thread_init(NULL);
    }
    
    User user;
    
    mloop = g_main_loop_new(NULL, FALSE);

    mutex = g_mutex_new();

    g_thread_create(thread1, &user, FALSE, NULL);
    g_thread_create(thread2, &user, FALSE, NULL);
    
    g_main_loop_run(mloop);
    
    return 0;
}
