/**
 *	@filename :
 * 		test_so3.c
 * 
 *  @note :
 * 		如果test_func_ptr同时定义在libtest_so3.c test_so3.c中， 会造成libtest_so3中的调用到test_so3.c的test_func_ptr,
 * 		(用gcc-4.8.2没有这个问题，但是用gcc-4.6.3就有这个问题)
 * 		解决方法：
 * 		(1). 把test_so3.c中的test_func_ptr定义成static
 * 		(2). 编译test_so3.c时把-fPIC去掉
 * 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dlfcn.h>

typedef void (*test_func_t)(void);

//test_func_t test_func_ptr;
static test_func_t test_func_ptr;
static test_func_t main_xxx_func;

int main(int argc, char *argv[]) {		
	int ret = 0;
	char *lib_path = "./bin/libtest_so3.so";
	const char *sym = "main_xxx";
	const char *sym1 = "test_func_ptr";
	//test_func_t *addr;
	unsigned int *addr;
	void *handle = NULL;
	char *error;
	
	printf("test_func_ptr = %p\n", &test_func_ptr);
	
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
