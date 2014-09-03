#include <stdio.h>

int main()      
{
	int a = 12 ;
	int b = func_a(a) + func_b(a) + func_c(a) + func_d(a) ;
	printf(" %d => %d\n" , a , b ) ;
	return 1; 	
}