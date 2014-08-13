/**
 *	@filename :
 * 		test_so3.c
 * 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dlfcn.h>

typedef void (*test_func_t)(void);

void (*main_xxx_func)(void);

void (*test_func_ptr)(void);

int main(int argc, char *argv[]) {		
	int ret = 0;
	char *lib_path = "./bin/libtest_so3.so";
	const char *sym = "main_xxx";
	const char *sym1 = "test_func_ptr";
	//test_func_t *addr;
	unsigned int *addr;
	void *handle = NULL;
	char *error;
	
	printf("lib path : %s\n", lib_path);
	
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

	addr = dlsym(handle, sym1);
	if ((error = dlerror()) != NULL) {
		fprintf(stderr, "%s\n", dlerror());		
		exit(1);
	}
	//printf("addr = %x\n", (unsigned int)addr);
	printf("addr = %p\n", addr);
	test_func_ptr = *(test_func_t *)addr;
	//printf("test_func_ptr = %x\n", (unsigned int)test_func_ptr);
	printf("test_func_ptr = %p\n", test_func_ptr);
	test_func_ptr();	
	
	printf("dlclose\n");
	dlclose(handle);
	printf("exit\n");
	
	return ret;
}
