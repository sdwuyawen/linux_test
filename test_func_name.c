/**
 * 	@filename :
 * 		test_func_name.c
 * 
 * 
 **/
#include <stdio.h>

#define _func	test_func

void _func(void) {
	printf("%s\n", __FUNCTION__);	
}

int main(void) {
	
	_func();
	
	return 0;
} 
