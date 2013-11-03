/**
 * 	@filename :
 * 		test_getpwnam.c
 * 
 * 	@note :
 *		// The passwd structure. 
 *		struct passwd {
 *			char     * pw_name;        // Username.       
 *			char     * pw_passwd;      // Password.       
 *			__uid_t -pw_uid;           // User ID.        
 *			__gid_t -pw_gid;           // Group ID.       
 *			char     * pw_gecos;       // Real name.      
 *			char     * pw_dir;         // Home directory. 
 *			char     * pw_shell;       //Shell program.   
 *		};
 *		在pwd.h中有passwd结构表示用户的属性的，getpwname函数能得到这个结构，我以为可以直接到到用户的password呢，
 * 		其实passwd就是/etc/passwd的一行信息组成的一个结构，唉，这样的getpwname函数其实我自己也能写啊。 
 **/
#include <stdio.h>
#include <pwd.h>

int main(int argc, char *argv[])
{
    struct passwd * pw;
    char *username = "zxl";
    
    if (argc > 1) {
		username = argv[1];
	}
	
    pw = getpwnam(username);
    if (!pw) {
        printf("%s is not exist\n", username);
        return -1;
    }

    printf("pw->pw_name   = %s\n", pw->pw_name);
    printf("pw->pw_passwd = %s\n", pw->pw_passwd);
    printf("pw->pw_uid    = %d\n", pw->pw_uid);
    printf("pw->pw_gid    = %d\n", pw->pw_gid);
    printf("pw->pw_gecos = %s\n", pw->pw_gecos);
    printf("pw->pw_dir    = %s\n", pw->pw_dir);
    printf("pw->pw_shell = %s\n", pw->pw_shell);
}
