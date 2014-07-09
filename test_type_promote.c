/*
 * 变量i被转换为无符号整型。这样一来，它的值不再是-1，而是size_t的最大值。变量i的类型之所以被转换，是因为sizeof操作符的返回类型是无符号的。
 * 具体参见C99/C11标准之常用算术转换一章：
 * 	若无符号整型的位数不低于另一操作数，则有符号数转为无符号数的类型 
 * 
 * C标准中，size_t被定义为不低于16位的无符号整型。通常size_t完全对应于long。这样一来，int和size_t的大小至少相等，可基于上述准则，强转为无符号整型 
 * C标准中“整型提升规则”中写道：
 * 	若int的表达范围足以覆盖所有的基础类型，此值将被转换为int；否则将转为unsigned int。这就叫做整型提升。整型提升过程中，所有其他的类型保持不变。
 */
#include <stdio.h>

void test_long(void)
{
    long i = -1;
 
	printf("%ld, %lu\n", i, sizeof(i));
    if (i < sizeof(i)) {	
         printf("OK\n");
    } else {
         printf("error\n");
    }	
}

int main(int argc, char *argv[])
{
	test_long();
 
    return 0;	
}
