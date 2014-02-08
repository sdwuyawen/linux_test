/**
 *	@filename :
 * 		test_sizeof.c
 * 
 *  @note :
 * 		32bit和64bit的模式下, int(4), char(1)的size不变， 只有long(4 or 8)的size出现变化, 指针变量(4 or 8)也有变化
 *      
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tag_test_st {
	int  i_test1;	
	char c_test1;	
	char c_test2;	
}test_st;

//__attribute__ ((packed))方式要注意每个成员变量的对齐，在某些平台可能因为类型不对齐而出现异常
typedef struct tag_test1_st {	
	int  i_test1;
	char c_test1;	
	char c_test2;	
}__attribute__ ((packed))test1_st;

int main(int argc, char *argv[]) 
{
	int status;
	long status1;
	char buffer[128];
	char *p;
	
	printf("sizeof(status) = %d, sizeof(int) = %d\n", (int)sizeof(status), (int)sizeof(int));
	printf("sizeof(buffer) = %d, sizeof(char) = %d\n", (int)sizeof(buffer), (int)sizeof(char));
	printf("sizeof(status1) = %d, sizeof(long) = %d\n", (int)sizeof(status1), (int)sizeof(long));	
	printf("sizeof(test_st) = %d, sizeof(test1_st) = %d\n", (int)sizeof(test_st), (int)sizeof(test1_st));
	printf("sizeof(p) = %d\n", (int)sizeof(p));
			
	return 0;
}
