/**
 *	@filename :
 * 		libtest_so.cpp
 * 
 * 	@note :
 * 		在cpp文件中定义的全局变量，编译之后符号没有变化, 而定义的函数有变化.
 * 		(1). Displays the entries in symbol table section of the file :
 * 			readelf -s ./bin/libtest_so.so
 * 		(2). Displays the contents of the file's dynamic section :
 * 			readelf -d ./bin/libtest_so.so
 **/
#include <stdio.h>

#include "lib_so.h"

/*
 * 
 */
#define TAG_NAME		"hello world"

//#define __STATIC_EN__

/*
 *
 */
int g_test_cnt = 0;

/*
 * 
 */  
#ifdef __STATIC_EN__ 
static int test_open(void *ptr) {
#else
int test_open(void *ptr) {
#endif	
	g_test_cnt++;
	
	return g_test_cnt;
}

/*
 * 
 */
#if 0
hal_module_info_t HAL_MODULE_INFO_SYM = {
	.tagname = TAG_NAME;
	.ver_major = 1;
	.ver_minor = 0;
	.open = test_open;
};

#else
hal_module_info_t HAL_MODULE_INFO_SYM = {
	tagname : TAG_NAME,
	ver_major : 1,
	ver_minor : 0,
	open : test_open,
};

#endif

