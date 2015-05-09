/**
 *	@filename :
 * 		test_so2_1.c
 * 
 * 	@note :
 * 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dlfcn.h>

#include "lib_so.h"

void (*main_xxx_func)(void);

void main_xxx(void)
{
	char *lib_path = "./bin/libtest_so2_1.so";
	const char *sym = "main_xxx";
	void *handle = NULL;
	char *error;
	
	printf("%s : lib path : %s\n", __FILE__, lib_path);
	
	dlerror();		//clear error code
	//handle = dlopen(lib_path, RTLD_LAZY);
	handle = dlopen(lib_path, RTLD_NOW);
	if (handle == NULL) {
		fprintf(stderr, "%s\n", dlerror());		
		exit(1);
	}
	
	main_xxx_func = dlsym(handle, sym);
	if ((error = dlerror()) != NULL) {
		fprintf(stderr, "%s\n", dlerror());		
		exit(1);
	}
	printf("libtest_inc_cnt = %p\n", libtest_inc_cnt);
	libtest_inc_cnt();
	printf("%s : main_xxx_func\n", __FILE__);
	main_xxx_func();	
	printf("%s : Wait key to close %s\n", __FILE__, lib_path);
	getchar();
	dlclose(handle);	
	printf("%s : exit\n", __FILE__);
}
