/**
 * 	@filename :
 * 		test_shell_eval.c
 * 
 * 	@note :
 * 	
 * 
 **/
#include <stdio.h>

int main(void)
{
	char *str = "package \'network\' \nconfig \'interface\' \'loopback\' \noption \'ifname\' \'lo\' \noption 'proto' 'static' \n";
	printf("%s\n", str);
}
