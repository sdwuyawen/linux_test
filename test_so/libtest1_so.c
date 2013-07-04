/**
 *	@filename :
 * 		libtest1_so.c
 * 
 * 	@note :
 * 		(1).如果本文件所在so中没有定义test_open()函数，会出现undefined symbol: test_open,
 * 		    不会调用test_so.c中的test_open.
 * 		(2).同样g_test_cnt也不会和test_so.c中的定义出现冲突.
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
#ifdef __EXTERN_EN__
extern int g_test_cnt;
#else
int g_test_cnt = 0;
#endif

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
#if 1
hal_module_info_t HAL_MODULE_INFO_SYM = {
	.tagname = TAG_NAME,
	.ver_major = 1,
	.ver_minor = 0,
	.open = test_open,
};

#else
hal_module_info_t HAL_MODULE_INFO_SYM = {
	tagname : TAG_NAME,
	ver_major : 1,
	ver_minor : 0,
	open : test_open,
};

#endif

