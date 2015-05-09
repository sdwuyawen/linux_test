/**
 *	@filename :
 * 		test_so2_1.c
1). test_so2_1 -> /bin/libtest_so2_2.so -> /bin/libtest_so2_1.so 
00400000-00401000 r-xp 00000000 08:06 8400685                            /home/paul2/work/test/linux_test/bin/test_so2_1
00600000-00601000 r--p 00000000 08:06 8400685                            /home/paul2/work/test/linux_test/bin/test_so2_1
00601000-00602000 rw-p 00001000 08:06 8400685                            /home/paul2/work/test/linux_test/bin/test_so2_1
0141d000-0143e000 rw-p 00000000 00:00 0                                  [heap]
7f81cb0d4000-7f81cb0d5000 r-xp 00000000 08:06 8400683                    /home/paul2/work/test/linux_test/bin/libtest_so2_1.so
7f81cb0d5000-7f81cb2d4000 ---p 00001000 08:06 8400683                    /home/paul2/work/test/linux_test/bin/libtest_so2_1.so
7f81cb2d4000-7f81cb2d5000 r--p 00000000 08:06 8400683                    /home/paul2/work/test/linux_test/bin/libtest_so2_1.so
7f81cb2d5000-7f81cb2d6000 rw-p 00001000 08:06 8400683                    /home/paul2/work/test/linux_test/bin/libtest_so2_1.so
7f81cb2d6000-7f81cb2d7000 r-xp 00000000 08:06 8400682                    /home/paul2/work/test/linux_test/bin/libtest_so2.so
7f81cb2d7000-7f81cb4d6000 ---p 00001000 08:06 8400682                    /home/paul2/work/test/linux_test/bin/libtest_so2.so
7f81cb4d6000-7f81cb4d7000 r--p 00000000 08:06 8400682                    /home/paul2/work/test/linux_test/bin/libtest_so2.so
7f81cb4d7000-7f81cb4d8000 rw-p 00001000 08:06 8400682                    /home/paul2/work/test/linux_test/bin/libtest_so2.so
7f81cb4d8000-7f81cb4d9000 r-xp 00000000 08:06 8400684                    /home/paul2/work/test/linux_test/bin/libtest_so2_2.so
7f81cb4d9000-7f81cb6d8000 ---p 00001000 08:06 8400684                    /home/paul2/work/test/linux_test/bin/libtest_so2_2.so
7f81cb6d8000-7f81cb6d9000 r--p 00000000 08:06 8400684                    /home/paul2/work/test/linux_test/bin/libtest_so2_2.so
7f81cb6d9000-7f81cb6da000 rw-p 00001000 08:06 8400684                    /home/paul2/work/test/linux_test/bin/libtest_so2_2.so
7f81cb6da000-7f81cb895000 r-xp 00000000 08:01 917852                     /lib/x86_64-linux-gnu/libc-2.19.so
7f81cb895000-7f81cba94000 ---p 001bb000 08:01 917852                     /lib/x86_64-linux-gnu/libc-2.19.so
7f81cba94000-7f81cba98000 r--p 001ba000 08:01 917852                     /lib/x86_64-linux-gnu/libc-2.19.so
7f81cba98000-7f81cba9a000 rw-p 001be000 08:01 917852                     /lib/x86_64-linux-gnu/libc-2.19.so
7f81cba9a000-7f81cba9f000 rw-p 00000000 00:00 0 
7f81cba9f000-7f81cbaa2000 r-xp 00000000 08:01 917725                     /lib/x86_64-linux-gnu/libdl-2.19.so
7f81cbaa2000-7f81cbca1000 ---p 00003000 08:01 917725                     /lib/x86_64-linux-gnu/libdl-2.19.so
7f81cbca1000-7f81cbca2000 r--p 00002000 08:01 917725                     /lib/x86_64-linux-gnu/libdl-2.19.so
7f81cbca2000-7f81cbca3000 rw-p 00003000 08:01 917725                     /lib/x86_64-linux-gnu/libdl-2.19.so
7f81cbca3000-7f81cbcc6000 r-xp 00000000 08:01 917848                     /lib/x86_64-linux-gnu/ld-2.19.so
7f81cbe93000-7f81cbe96000 rw-p 00000000 00:00 0 
7f81cbec1000-7f81cbec5000 rw-p 00000000 00:00 0 
7f81cbec5000-7f81cbec6000 r--p 00022000 08:01 917848                     /lib/x86_64-linux-gnu/ld-2.19.so
7f81cbec6000-7f81cbec7000 rw-p 00023000 08:01 917848                     /lib/x86_64-linux-gnu/ld-2.19.so
7f81cbec7000-7f81cbec8000 rw-p 00000000 00:00 0 
7ffc0b2d8000-7ffc0b2f9000 rw-p 00000000 00:00 0                          [stack]
7ffc0b315000-7ffc0b317000 r-xp 00000000 00:00 0                          [vdso]
ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0                  [vsyscall]

2). test_so2_1 -> /bin/libtest_so2_2.so
00400000-00401000 r-xp 00000000 08:06 8400685                            /home/paul2/work/test/linux_test/bin/test_so2_1
00600000-00601000 r--p 00000000 08:06 8400685                            /home/paul2/work/test/linux_test/bin/test_so2_1
00601000-00602000 rw-p 00001000 08:06 8400685                            /home/paul2/work/test/linux_test/bin/test_so2_1
0141d000-0143e000 rw-p 00000000 00:00 0                                  [heap]
7f81cb2d6000-7f81cb2d7000 r-xp 00000000 08:06 8400682                    /home/paul2/work/test/linux_test/bin/libtest_so2.so
7f81cb2d7000-7f81cb4d6000 ---p 00001000 08:06 8400682                    /home/paul2/work/test/linux_test/bin/libtest_so2.so
7f81cb4d6000-7f81cb4d7000 r--p 00000000 08:06 8400682                    /home/paul2/work/test/linux_test/bin/libtest_so2.so
7f81cb4d7000-7f81cb4d8000 rw-p 00001000 08:06 8400682                    /home/paul2/work/test/linux_test/bin/libtest_so2.so
7f81cb4d8000-7f81cb4d9000 r-xp 00000000 08:06 8400684                    /home/paul2/work/test/linux_test/bin/libtest_so2_2.so
7f81cb4d9000-7f81cb6d8000 ---p 00001000 08:06 8400684                    /home/paul2/work/test/linux_test/bin/libtest_so2_2.so
7f81cb6d8000-7f81cb6d9000 r--p 00000000 08:06 8400684                    /home/paul2/work/test/linux_test/bin/libtest_so2_2.so
7f81cb6d9000-7f81cb6da000 rw-p 00001000 08:06 8400684                    /home/paul2/work/test/linux_test/bin/libtest_so2_2.so
7f81cb6da000-7f81cb895000 r-xp 00000000 08:01 917852                     /lib/x86_64-linux-gnu/libc-2.19.so
7f81cb895000-7f81cba94000 ---p 001bb000 08:01 917852                     /lib/x86_64-linux-gnu/libc-2.19.so
7f81cba94000-7f81cba98000 r--p 001ba000 08:01 917852                     /lib/x86_64-linux-gnu/libc-2.19.so
7f81cba98000-7f81cba9a000 rw-p 001be000 08:01 917852                     /lib/x86_64-linux-gnu/libc-2.19.so
7f81cba9a000-7f81cba9f000 rw-p 00000000 00:00 0 
7f81cba9f000-7f81cbaa2000 r-xp 00000000 08:01 917725                     /lib/x86_64-linux-gnu/libdl-2.19.so
7f81cbaa2000-7f81cbca1000 ---p 00003000 08:01 917725                     /lib/x86_64-linux-gnu/libdl-2.19.so
7f81cbca1000-7f81cbca2000 r--p 00002000 08:01 917725                     /lib/x86_64-linux-gnu/libdl-2.19.so
7f81cbca2000-7f81cbca3000 rw-p 00003000 08:01 917725                     /lib/x86_64-linux-gnu/libdl-2.19.so
7f81cbca3000-7f81cbcc6000 r-xp 00000000 08:01 917848                     /lib/x86_64-linux-gnu/ld-2.19.so
7f81cbe93000-7f81cbe96000 rw-p 00000000 00:00 0 
7f81cbec1000-7f81cbec5000 rw-p 00000000 00:00 0 
7f81cbec5000-7f81cbec6000 r--p 00022000 08:01 917848                     /lib/x86_64-linux-gnu/ld-2.19.so
7f81cbec6000-7f81cbec7000 rw-p 00023000 08:01 917848                     /lib/x86_64-linux-gnu/ld-2.19.so
7f81cbec7000-7f81cbec8000 rw-p 00000000 00:00 0 
7ffc0b2d8000-7ffc0b2f9000 rw-p 00000000 00:00 0                          [stack]
7ffc0b315000-7ffc0b317000 r-xp 00000000 00:00 0                          [vdso]
ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0                  [vsyscall]

3).打印输出
lib path : ./bin/libtest_so2_2.so
18126 libtest_init: 1
main_xxx_func
libtest_so2_2.c : lib path : ./bin/libtest_so2_1.so
libtest_inc_cnt = 0x7f81cb2d6847
libtest_so2_2.c : main_xxx_func
libtest_inc_cnt = 0x7f81cb2d6847
libtest_so2_1.c : 2
libtest_so2_2.c : Wait key to close ./bin/libtest_so2_1.so

libtest_so2_2.c : exit
test_so2_1.c : Wait key to close ./bin/libtest_so2_2.so

dlclose
18126 libtest_uninit: 1

exit

4). 如果libtest_so2_2.so链接libtest_so2.so, 而libtest_so2_1.so不链接libtest_so2.so, 运行报错:
./bin/libtest_so2_1.so: undefined symbol: libtest_inc_cnt
因为libtest_so2_1.so不知道去哪里查找libtest_inc_cnt(全局表没有这个符号), 除非test_so2_1中的dlopen
加RTLD_GLOBAL
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
	const char *sym = "main_xxx";
	void *handle = NULL;
	char *error;
	
	printf("lib path : %s\n", lib_path);
	
	dlerror();		//clear error code
	//handle = dlopen(lib_path, RTLD_LAZY);
	handle = dlopen(lib_path, RTLD_NOW);
	//handle = dlopen(lib_path, RTLD_NOW|RTLD_GLOBAL);
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
	printf("%s : Wait key to close %s\n", __FILE__, lib_path);
	getchar();
	printf("dlclose\n");
	dlclose(handle);
	getchar();
	printf("exit\n");
	
	return ret;
}
