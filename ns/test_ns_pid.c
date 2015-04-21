/**
 * 	@filename :
 * 		test_ns_pid.c
 * 
 * 	@note :
 * 		通过CLONE_NEWIPC来PID namespace隔离。
 * 
paul@wubaoqiang:~/work/test/linux_test$ sudo ./bin/test_ns_pid 
程序开始: 
在子进程中!
root@Changed Name:~/work/test/linux_test# echo $$
1
root@Changed Name:~/work/test/linux_test# exit
exit
已退出
paul@wubaoqiang:~/work/test/linux_test$ echo $$
2603
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
  int child_pid = clone(child_main, child_stack + STACK_SIZE, CLONE_NEWPID | CLONE_NEWIPC | CLONE_NEWUTS | SIGCHLD, NULL);
  if (child_pid < 0) {
	  goto _exit;
  }
  waitpid(child_pid, NULL, 0);
_exit:  
  printf("已退出\n");
  return 0;
} 
