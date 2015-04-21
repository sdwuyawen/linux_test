/**
 * 	@filename :
 * 		test_namespace.c
 * 
 * 	@note :
	1. no CLONE_NEWUTS
paul@wubaoqiang:~/work/test/linux_test$ sudo ./bin/test_namespace 
程序开始: 
在子进程中!
root@Changed Name:~/work/test/linux_test# uname -a
Linux Changed Name 3.13.0-49-generic #83-Ubuntu SMP Fri Apr 10 20:11:33 UTC 2015 x86_64 x86_64 x86_64 GNU/Linux
root@Changed Name:~/work/test/linux_test# exit
exit
已退出
paul@wubaoqiang:~/work/test/linux_test$ hostname
Changed Name
paul@wubaoqiang:~/work/test/linux_test$

	2. CLONE_NEWUTS
paul@wubaoqiang:~/work/test/linux_test$ sudo ./bin/test_namespace 
[sudo] password for paul: 
程序开始: 
在子进程中!
root@Changed Name:~/work/test/linux_test# uname -a
Linux Changed Name 3.13.0-49-generic #83-Ubuntu SMP Fri Apr 10 20:11:33 UTC 2015 x86_64 x86_64 x86_64 GNU/Linux
root@Changed Name:~/work/test/linux_test# exit
exit
已退出
paul@wubaoqiang:~/work/test/linux_test$ hostname
wubaoqiang
paul@wubaoqiang:~/work/test/linux_test$
 * 
 **/
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];
char* const child_args[] = {
  "/bin/bash",
  NULL
};

int child_main(void* args) {
  printf("在子进程中!\n");
  sethostname("Changed Namespace", 12);
  execv(child_args[0], child_args);
  return 1;
}

int main() {
  printf("程序开始: \n");
  //int child_pid = clone(child_main, child_stack + STACK_SIZE, SIGCHLD, NULL);
  int child_pid = clone(child_main, child_stack + STACK_SIZE, CLONE_NEWUTS | SIGCHLD, NULL);
  if (child_pid < 0) {
	  goto _exit;
  }
  waitpid(child_pid, NULL, 0);
_exit:  
  printf("已退出\n");
  return 0;
} 
