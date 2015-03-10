/*
 * 	@filename :
 * 		test_prefix.c
 * 
 */
#include <stdio.h>

/* */
#define PREFIX 		aa_
#define _FUNC(a,b) 	(a##b)		
#define FUNC(a,b) 	_FUNC(a,b)	//加多一层中间转换宏. 加这层宏的用意是把所有宏的参数(如PREFIX)在这层里全部展开

/* */
#define test	FUNC(PREFIX,test)

/* */ 
int test()
{
	printf("%s\n", __FUNCTION__);
	
	return 0;
}

/* */ 
int main() 
{
	int ret;
	
	ret = test();
	
	return 0;
} 
