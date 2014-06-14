/**
   @filename :
   		test_setsid.c
**/
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

/*  在父进程(是会话的领头进程)调用setsid()会返回-1, 而在子进程调用它, 返回新会话的ID;
 *	调用setsid函数的进程成为新的会话的领头进程，并与其父进程的会话组和进程组脱离。由于会话对控制终端的独占性，进程同时与控制终端脱离。
 *  在控制终端输入ctrl+c, 只会kill掉父进程， 子进程还可以运行。
 */ 
/**
 **/
#define TEST_TIMEOUT 50

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
		  int ret = setsid();
		  printf("setsid ret=%d\n", ret);
		  while (timeout-- > 0) {
			   sleep(1);
			   printf("remain %d sec\n", timeout);
		  }
		  printf("test_child exit\n");
		  return 0;
	 }
		  
	 prctl(PR_SET_NAME, "test_parent", NULL, NULL, NULL);	 
	 printf("wait child...\n");
	 waitpid(pid, NULL, 0);
	 printf("test_parent exit\n");
	 
	 printf("=== %s end  ===\n", argv[0]);
	 
	 return 0;
}
