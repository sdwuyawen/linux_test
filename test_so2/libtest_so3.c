/**
 *	@filename :
 * 		libtest_so3.c
 * 
 * 	@note :
 * 
 **/
#include <stdio.h>

int g_test_cnt = 0;

void (*test_func_ptr)(void);

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

void libtest_main(void)
{
	printf("libtest_main\n");
}

void main_xxx(void)
{
	//printf("test_func_ptr = %x\n", (unsigned int)&test_func_ptr);
	printf("test_func_ptr = %p\n", &test_func_ptr);
	test_func_ptr = libtest_main;
	//printf("libtest_main=%x\n", (unsigned int)libtest_main);
	printf("libtest_main=%p\n", libtest_main);
	//printf("test_func_ptr = %x\n", (unsigned int)test_func_ptr);
	printf("test_func_ptr = %p\n", test_func_ptr);
	
	//test_func_ptr();
}
