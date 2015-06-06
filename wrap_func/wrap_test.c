#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 */
extern void libc_printf(char *str);
extern void libcc_printf(char *str);

//
extern void wrap_malloc_info(void);
extern void wrap_malloc_debug(int enable);

/*
 */
int main(int argc, char *argv[])
{
	const char *str = "hello world";
	char *new_str = NULL;
	
	wrap_malloc_debug(1);
	
	if (argc > 1) {
		libc_printf(argv[1]);
		libcc_printf(argv[1]);
	}
	
	char *ptr = NULL;
	char *new_ptr = NULL;
	
	ptr = malloc(20);
	if (ptr != NULL) {
		wrap_malloc_info();
		new_ptr = realloc(ptr, 25);
		if (new_ptr != NULL) {
			ptr = NULL;
		}
	}
		
	ptr = calloc(8, 16);
	if (ptr != NULL) {
		printf("calloc OK!\n");
	}
	wrap_malloc_info();
	
	new_str = strdup(str);
	if (new_str != NULL) {
		printf("new_str = %s\n", new_str);
	}
	
	wrap_malloc_info();
	if (new_ptr != NULL) {
		free(new_ptr);
	}		
	
	if (ptr != NULL) {
		free(ptr);
	}
	
	if (new_str != NULL) {
		free(new_str);
	}
	
	return 0;
}
