/**
 *	@filename :
 * 		test_so.c
 * 
 * 	@note :
 * 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "lib_so.h"

int main(int argc, char *argv[]) {			
	printf("%s : %d\n", argv[0], libtest_get_cnt());
	
	//while (1) {	//为了查看maps
		//sleep(1);
	//}
	
	return 0;
}
