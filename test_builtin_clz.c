/**
   @filename :
		test_builtin_clz.c
		
   @noted :
		__builtin_clz(a) check bit set from MSB to LSB.
**/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>


int main(int argc, char *argv[])
{
	int value;
	int i;
	
	if (argc < 2) {
		printf("error : not input value\n");
		return -1;
	}

	value = strtol(argv[1], 0, 16);

	while (value) {
		i = 31 - __builtin_clz(value);

		value &= ~(1<<i);

		printf("[%d] = 0x%x\n", i, value);
	}
    
    return 0;
}

