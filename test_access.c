/**
 * @filename :
 * 	test_access.c
 * 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define FILE_OFFSET		2
#define PERM_NUM		3

static char *perm_str[PERM_NUM] = {
	"X_OK",
	"W_OK",
	"R_OK"
};

int main(int argc, char *argv[]) 
{				
	if (argc > FILE_OFFSET) {
		int i;
		int flags;				
		int index = FILE_OFFSET;
		
		flags = atoi(argv[1]);
		printf("===============================\n");
		printf("permission : \033[33;1m");
		for (i=0; i<PERM_NUM; i++) {
			if (flags & (1<<i)) {
				printf("%s ", perm_str[i]);
			}
		}
		printf("\033[0m\n");
		printf("===============================\n");
		while (index < argc) {
			if (access(argv[index], flags) != -1) {			
				printf("\033[34;1m [%s] OK\033[0m\n", argv[index]);
			} else {
				printf("\033[35;1m [%s] FAIL\033[0m\n", argv[index]);
			}
			index++;
		}
		printf("===============================\n");
	} else {
		printf("Usage : test_access flags file1 [file2...]\n");
		printf("  flags:\n");
		printf("        4 - R_OK\n");
		printf("        2 - W_OK\n");
		printf("        1 - X_OK\n");
	}
	
	return 0;
}

