#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC_SIZE		256

extern "C" void libcc_printf(char *str);

void libcc_printf(char *str)
{
	char *ptr = new char[strlen(str) + MALLOC_SIZE];
	
	if (ptr != NULL) {
		sprintf(ptr, "%s : %s", __func__, str);
		printf("%s\n", ptr);
		delete ptr;
	}
	
	return;
}
