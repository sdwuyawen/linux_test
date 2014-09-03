#include <stdio.h>

int main()
{
	int arg = 11;
	int res =  func_a(arg)+func_b(arg)+func_c(arg)+func_d(arg);
	printf(" %d => %d \n" , arg , res );
	return res;
}