#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 */
extern void libc_printf(char *str);
extern void libcc_printf(char *str);
 
/*
 */
void *__real_malloc(size_t size);
void *__wrap_malloc(size_t size)
{
	printf("__wrap_malloc\n");
	return __real_malloc(size);
}

void __real_free(void *ptr);
void __wrap_free(void *ptr)
{
	printf("__wrap_free\n");
	__real_free(ptr);
}

/*
 */
int main(int argc, char *argv[])
{
	if (argc > 1) {
		libc_printf(argv[1]);
		libcc_printf(argv[1]);
	}	
	
	return 0;
}
