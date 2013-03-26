/**
   @filename :
   		test_getprop.c
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/mman.h>

/**
 **/
int main(int argc, char *argv[]) {
	 int fd;
	 int size;
	 char *env;
	 void *pa;

	 env = (char *)getenv((char *)"TEST_PROP");
	 if (!env) {
		  printf("no env\n");
		  return -1;
	 }
	 
	 fd = atoi(env);
	 env = strchr(env, ',');
	 if (!env) {
		  printf("env error!\n");
		  return -1;
	 }
	 size = atoi(env + 1);
    
	 pa = mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);	 
	 if(pa == MAP_FAILED) {
		  printf("mmap error!\n");
		  close(fd);
		  return -1;
	 }

	 printf("%s : %s\n", argv[0], (char *)pa);

	 getchar();
	 munmap(pa, size);
	 close(fd);
	 
	 return 0;
}

