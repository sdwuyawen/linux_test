/**
   @filename :
   		test_fork.c
**/
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

/**
 **/
#define TEST_TIMEOUT 15

int main(int argc, char *argv[])
{
	 pid_t pid;
	 
	 printf("=== %s start ===\n", argv[0]);

	 pid = fork();
	 if (pid < 0) {
		  return -1;
	 }
	 
	 if (pid == 0) {
		  int timeout = TEST_TIMEOUT;
		  prctl(PR_SET_NAME, "test_child", NULL, NULL, NULL);
		  while (timeout-- > 0) {
			   sleep(1);
			   printf("remain %d sec\n", timeout);
		  }
		  printf("test_child exit\n");
		  return 0;
	 }
		  
	 prctl(PR_SET_NAME, "test_parent", NULL, NULL, NULL);
	 waitpid(pid, NULL, 0);
	 printf("test_parent exit\n");
	 
	 printf("=== %s end  ===\n", argv[0]);
	 
	 return 0;
}
