/**
 * 	@filename :
 * 		test_ns_ns.c
 * 
 * 	@note :
 * 		通过CLONE_NEWNS来Mount namespace隔离, 不会影响root namespace文件系统。
 * 
paul@wubaoqiang:~/work/test/linux_test$ sudo ./bin/test_ns_ns
程序开始: 
在子进程中!
root@Changed Name:~/work/test/linux_test# mount -t proc proc /proc
root@Changed Name:~/work/test/linux_test# ps a
  PID TTY      STAT   TIME COMMAND
    1 pts/4    S      0:00 /bin/bash
   15 pts/4    R+     0:00 ps a
root@Changed Name:~/work/test/linux_test# exit
exit
已退出
paul@wubaoqiang:~/work/test/linux_test$ ps a
  PID TTY      STAT   TIME COMMAND
  965 tty4     Ss+    0:00 /sbin/getty -8 38400 tty4
  969 tty5     Ss+    0:00 /sbin/getty -8 38400 tty5
  977 tty2     Ss+    0:00 /sbin/getty -8 38400 tty2
  978 tty3     Ss+    0:00 /sbin/getty -8 38400 tty3
  982 tty6     Ss+    0:00 /sbin/getty -8 38400 tty6
 1348 tty7     Ssl+   2:36 /usr/bin/X -core :0 -seat seat0 -auth /var/run/lightdm/root/:0 -nolisten tcp vt7 -novtswitch
 1387 tty1     Ss+    0:00 /sbin/getty -8 38400 tty1
 2603 pts/4    Ss     0:00 bash
 9539 pts/4    R+     0:00 ps a
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
  int child_pid = clone(child_main, child_stack + STACK_SIZE, CLONE_NEWNS | CLONE_NEWPID | CLONE_NEWIPC | CLONE_NEWUTS | SIGCHLD, NULL);
  if (child_pid < 0) {
	  goto _exit;
  }
  waitpid(child_pid, NULL, 0);
_exit:  
  printf("已退出\n");
  return 0;
} 
