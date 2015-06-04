#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC_SIZE		256

void libc_printf(char *str)
{
	char *ptr = malloc(strlen(str) + MALLOC_SIZE);
	
	if (ptr != NULL) {
		sprintf(ptr, "%s : %s", __func__, str);
		printf("%s\n", ptr);
		free(ptr);
	}
	
	return;
}
