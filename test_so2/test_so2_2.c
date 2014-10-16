/**
 *	@filename :
 * 		test_so2_1.c
 * 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dlfcn.h>

#include "lib_so.h"

void (*main_xxx_func)(void);

int main(int argc, char *argv[]) {		
	int ret = 0;
	char *lib_path = "./bin/libtest_so2_2.so";
	char *lib1_path = "./bin/libtest_so2.so";
	const char *sym = "main_xxx";
	void *handle = NULL;
	void *handle1 = NULL;
	char *error;
	
	printf("lib path : %s\n", lib_path);
	
	dlerror();		//clear error code
	handle1 = dlopen(lib1_path, RTLD_NOW|RTLD_GLOBAL);		//提供libtest_inc_cnt函数给libtest_so2_2,libtest_so2_1访问
	
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
	printf("main_xxx_func\n");
	main_xxx_func();
	getchar();
	printf("dlclose\n");
	dlclose(handle);
	dlclose(handle1);
	getchar();
	printf("exit\n");
	
	return ret;
}
