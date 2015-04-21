/**
 * 	@filename :
 * 		test_ns_ipc.c
 * 
 * 	@note :
 * 		容器中进程间通信采用的方法包括常见的信号量、消息队列和共享内存, 通过CLONE_NEWIPC来隔离。
 * 
paul@wubaoqiang:~/work/test/linux_test$ sudo ./bin/test_ns_ipc 
程序开始: 
在子进程中!
root@Changed Name:~/work/test/linux_test# ipcs -q

--------- 消息队列 -----------
键        msqid      拥有者  权限     已用字节数 消息      

root@Changed Name:~/work/test/linux_test# exit
exit
已退出
paul@wubaoqiang:~/work/test/linux_test$ ipcs -q

--------- 消息队列 -----------
键        msqid      拥有者  权限     已用字节数 消息      
0x2f213167 0          root       644        0            0           

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
  int child_pid = clone(child_main, child_stack + STACK_SIZE, CLONE_NEWIPC | CLONE_NEWUTS | SIGCHLD, NULL);
  if (child_pid < 0) {
	  goto _exit;
  }
  waitpid(child_pid, NULL, 0);
_exit:  
  printf("已退出\n");
  return 0;
} 
