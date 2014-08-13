/**
 *	@filename :
 * 		test_so2.c
 * 
 * 	@note :
 * 	1). test_so2和test_so2_1.so都编译链接libtest_so2.so, 程序中只有一份libtest_so2.so.
 * 		(1).在main_xxx_func()之前调用了libtest_inc_cnt(), libtest_so2.so在主程序中加载和卸载.
		4687 libtest_init: 1
		lib path : ./bin/libtest_so2_1.so
		main_xxx_func
		libtest_so2_1.c : 2

		dlclose

		exit
		4687 libtest_uninit: 1
		
		(2). 如果没有调用libtest_inc_cnt()，libtest_so2.so只会在dlopen中去加载，并在dlclose中卸载
		lib path : ./bin/libtest_so2_1.so
		4822 libtest_init: 1
		main_xxx_func
		libtest_so2_1.c : 1

		dlclose
		4822 libtest_uninit: 0

		exit

 *  2). 只有test_so2_1.so编译链接libtest_so2.so
 * 		在调用dlclose之后，libtest_so2.so也会被卸载掉.
		lib path : ./bin/test_so2_1.so
		4306 libtest_init: 1
		main_xxx_func
		test_so2_1.c : 1

		dlclose
		4306 libtest_uninit: 0

		exit
		
 * 	3). 通过libtest_so2_2.so在调用libtest_so2_1.so	
 * 		(1). /bin/test_so2
		7503 libtest_init: 1
		lib path : ./bin/libtest_so2_2.so
		main_xxx_func
		libtest_so2_2.c : lib path : ./bin/libtest_so2_1.so
		libtest_so2_2.c : main_xxx_func
		libtest_so2_1.c : 3
		libtest_so2_2.c : exit

		dlclose

		exit
		7503 libtest_uninit: 2	 		
 * 		(2). /bin/test_so2_1, 在需要用的时候才去加载		 
		lib path : ./bin/libtest_so2_2.so
		7799 libtest_init: 1
		main_xxx_func
		libtest_so2_2.c : lib path : ./bin/libtest_so2_1.so
		libtest_so2_2.c : main_xxx_func
		libtest_so2_1.c : 2
		libtest_so2_2.c : exit

		dlclose
		7799 libtest_uninit: 1

		exit
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
	const char *sym = "main_xxx";
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
	libtest_inc_cnt();
	printf("main_xxx_func\n");
	main_xxx_func();
	getchar();
	printf("dlclose\n");
	dlclose(handle);
	getchar();
	printf("exit\n");
	
	return ret;
}
