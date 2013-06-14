/**
   @filename :
		test_builtin_clz.c
		
   @noted :
    int __builtin_ffs (unsigned int x)
    ����x�����һλ1���ǴӺ���ǰ�ڼ�λ������7368��1110011001000������4��
    int __builtin_clz (unsigned int x)
    ����ǰ����0�ĸ�����
    int __builtin_ctz (unsigned int x)
    ���غ����0����������__builtin_clz��ԡ�
    int __builtin_popcount (unsigned int x)
    ���ض����Ʊ�ʾ��1�ĸ�����
    int __builtin_parity (unsigned int x)
    ����x����żУ��λ��Ҳ����x��1�ĸ���ģ2�Ľ����

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

