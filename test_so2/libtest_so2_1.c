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
	printf("%s : %d\n", __FILE__, libtest_get_cnt());
}
