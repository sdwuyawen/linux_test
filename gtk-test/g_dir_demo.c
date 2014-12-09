#include <glib.h>

void listDir(const gchar *parent, int hier) {
    const gchar *file, *fullPath;
    GDir *dir;
    int i;
    
    dir = g_dir_open(parent, 0, NULL);
    
    while((file = g_dir_read_name(dir))) {
        for(i = 0; i < hier; i++) {
            g_print("    ");
        }
        
        fullPath = g_build_filename(parent, file, NULL);
        if(g_file_test(fullPath, G_FILE_TEST_IS_DIR)) {
           g_print("[%s]\n", file);
           listDir(fullPath, hier + 1);
        }
        else {
            g_print("%s\n", file);
        }
    }
    
    g_dir_close(dir);
}

int main(int argc, char *argv[]) {   
	if (argc < 2) {
		g_print("CMD : %s <dir>\n", argv[0]);
		return 1;
	}  
	
    listDir(argv[1], 0);
    return 0;
}
