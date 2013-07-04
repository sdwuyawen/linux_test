/**
 *	@filename :
 * 		libtest_so1.c
 * 
 * 	@note :
 * 
 **/
#include <stdio.h>

#include "lib_so.h"

static int g_test_cnt = 0;

/*
 * 
 */
static __attribute__((constructor)) void libtest_init(void);
static __attribute__((destructor)) void libtest_uninit(void);

/*
 *
 */
static void libtest_init(void) {	
	g_test_cnt++;
	printf("%d %s: %d\n", getpid(), __FUNCTION__, g_test_cnt);
}

/*
 *
 */
static void libtest_uninit(void) {
	g_test_cnt--;
	printf("%d %s: %d\n", getpid(), __FUNCTION__, g_test_cnt);
}

/*
 *
 */
int libtest_get_cnt(void) {
	return g_test_cnt;
}

/*
 *
 */  
int libtest_inc_cnt(void) {
	g_test_cnt++;
	return g_test_cnt;
}

/*
 *
 */
int libtest_dec_cnt(void) {
	g_test_cnt--;
	return g_test_cnt;
}
