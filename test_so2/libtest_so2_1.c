/**
 *	@filename :
 * 		test_so2_1.c
 * 
 * 	@note :
 * 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "lib_so.h"

void main_xxx(void)
{
	printf("libtest_inc_cnt = %p\n", libtest_inc_cnt);
	printf("%s : %d\n", __FILE__, libtest_get_cnt());
}
