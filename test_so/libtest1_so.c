/**
 *	@filename :
 * 		libtest1_so.c
 * 
 * 	@note :
 * 		如果本文件中没有定义test_open()函数，才会调用test_so.c中的定义的test_open()函数.
 * 
 **/
#include <stdio.h>

#include "lib_so.h"

/*
 * 
 */
#define TAG_NAME		"hello world"

//#define __EXTERN_EN__
//#define __STATIC_EN__

/*
 *
 */
int g_test_cnt = 0;

/*
 * 
 */ 
#ifdef __EXTERN_EN__
extern int test_open(void *ptr);	

#else
#ifdef __STATIC_EN__
static int test_open(void *ptr) {
#else
int test_open(void *ptr) {
#endif
	g_test_cnt++;
	return g_test_cnt;
}

#endif

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

