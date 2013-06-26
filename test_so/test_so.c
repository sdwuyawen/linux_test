/**
 *	@filename :
 * 		test_so.c
 * 
 * 	@note :
 * 		符号先在本地查找解决.
 * 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "lib_so.h"

int g_test_cnt = 0;

int test_open(void *ptr) {
	g_test_cnt += 2;
	
	return g_test_cnt;
}

int main(int argc, char *argv[]) {
	char *lib_path = "libtest.so";
	void *handle = NULL;
	const char *sym = HAL_MODULE_INFO_SYM_STR;
	hal_module_info_t *hmi;
	char *error;
	
	if (argc > 1) {
		lib_path = argv[1];
	}
	printf("lib path : %s\n", lib_path);
	
	dlerror();		//clear error code
	handle = dlopen(lib_path, RTLD_LAZY);
	//lib_ptr = dlopen(path_so, RTLD_NOW);
	if (handle == NULL) {
		fputs (dlerror(), stderr);
		exit(1);
	}
	
	hmi = (hal_module_info_t *)dlsym(handle, sym);
	if ((error = dlerror()) != NULL) {
		fputs (error, stderr);
		exit(1);
	}
	
	//info
	printf("========= lib info =========\n");
	printf("%s : %d, %d\n", hmi->tagname, hmi->ver_major, hmi->ver_minor);
	printf("%d, %d\n", hmi->open(NULL), g_test_cnt);
	
	dlclose(handle);
		
	return 0;
}
