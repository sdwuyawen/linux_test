/**
   @filename :
		test_builtin_clz.c
		
   @noted :
    int __builtin_ffs (unsigned int x)
    返回x的最后一位1的是从后向前第几位，比如7368（1110011001000）返回4。
    int __builtin_clz (unsigned int x)
    返回前导的0的个数。
    int __builtin_ctz (unsigned int x)
    返回后面的0个个数，和__builtin_clz相对。
    int __builtin_popcount (unsigned int x)
    返回二进制表示中1的个数。
    int __builtin_parity (unsigned int x)
    返回x的奇偶校验位，也就是x的1的个数模2的结果。

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

