/**
 * @filename :
 * 		test_pid_ns.c
 * 
 * @note :
 * 	namespace其实就是给虚拟化用的，PID namespace其实就是建立一个新的PID空间，这样内部可以使用一套新的PID，而且不会和外部冲突。
 * 	这也就是说某个进程其实会有两个PID，一个空间一个。
 *   
 * 	$sudo ./test_pid_ns 3
 *flags = CLONE_NEWPID | CLONE_NEWNS;
 *   
 * 		Out of the container, my pid is: 12153
 *		cpid: 12155
 *		Parent sleeping 20 seconds
 *		In the container, my pid is: 1
 *		pid of my child is 2
 *		pid of my child is 3
 *		pid of my child is 4			
 * 
 * flags = CLONE_NEWPID
 *
 *		Out of the container, my pid is: 12314
 *		cpid: 12315
 *		Parent sleeping 20 seconds
 *		In the container, my pid is: 1
 *		pid of my child is 2
 *		pid of my child is 3
 *		pid of my child is 4 
 * 
 * flags = 
 * 
 * 		Out of the container, my pid is: 12599
 *		cpid: 12601
 *		Parent sleeping 20 seconds
 *		In the container, my pid is: 12601
 *		pid of my child is 12602
 *		pid of my child is 12603
 *		pid of my child is 12604
 * 
 **/
#if 0
int clone(int (*fn)(void *), void *child_stack, int flags, void *arg);
这里fn是函数指针，我们知道进程的4要素，这个就是指向程序的指针，就是所谓的“剧本", child_stack明显是为子进程分配系统堆栈空间（在linux下系统堆栈空间是2页面，就是8K的内存，其中在这块内存中，低地址上放入了值，这个值就是进程控制块task_struct的值）,flags就是标志用来描述你需要从父进程继承那些资源， arg就是传给子进程的参数）。下面是flags可以取的值
标志                   含义
/*
 * cloning flags:
 */
#define CSIGNAL		0x000000ff	/* signal mask to be sent at exit */
#define CLONE_VM	0x00000100	/* set if VM shared between processes */
#define CLONE_FS	0x00000200	/* set if fs info shared between processes */
#define CLONE_FILES	0x00000400	/* set if open files shared between processes */
#define CLONE_SIGHAND	0x00000800	/* set if signal handlers and blocked signals shared */
#define CLONE_PTRACE	0x00002000	/* set if we want to let tracing continue on the child too */
#define CLONE_VFORK	0x00004000	/* set if the parent wants the child to wake it up on mm_release */
#define CLONE_PARENT	0x00008000	/* set if we want to have the same parent as the cloner */
#define CLONE_THREAD	0x00010000	/* Same thread group? */
#define CLONE_NEWNS	0x00020000	/* New namespace group? */
#define CLONE_SYSVSEM	0x00040000	/* share system V SEM_UNDO semantics */
#define CLONE_SETTLS	0x00080000	/* create a new TLS for the child */
#define CLONE_PARENT_SETTID	0x00100000	/* set the TID in the parent */
#define CLONE_CHILD_CLEARTID	0x00200000	/* clear the TID in the child */
#define CLONE_DETACHED		0x00400000	/* Unused, ignored */
#define CLONE_UNTRACED		0x00800000	/* set if the tracing process can't force CLONE_PTRACE on this clone */
#define CLONE_CHILD_SETTID	0x01000000	/* set the TID in the child */
/* 0x02000000 was previously the unused CLONE_STOPPED (Start in stopped state)
   and is now available for re-use. */
#define CLONE_NEWUTS		0x04000000	/* New utsname group? */
#define CLONE_NEWIPC		0x08000000	/* New ipcs */
#define CLONE_NEWUSER		0x10000000	/* New user namespace */
#define CLONE_NEWPID		0x20000000	/* New pid namespace */
#define CLONE_NEWNET		0x40000000	/* New network namespace */
#define CLONE_IO		0x80000000	/* Clone io context */
 
下面的例子是创建一个线程（子进程共享了父进程虚存空间，没有自己独立的虚存空间不能称其为进程）。父进程被挂起当子线程释放虚存资源后再继续执行。
与系统调用clone功能相似的系统调用有fork,但fork事实上只是clone的功能的一部分，clone与fork的主要区别在于传递了几个参数，而当中最重要的参数就是conle_flags,下表是系统定义的几个clone_flags标志：
标志 Value 含义
 CLONE_VM 0x00000100 置起此标志在进程间共享地址空间
 CLONE_FS 0x00000200 置起此标志在进程间共享文件系统信息
 CLONE_FILES 0x00000400 置起此标志在进程间共享打开的文件
 CLONE_SIGHAND 0x00000800 置起此标志在进程间共享信号处理程序
如果置起以上标志所做的处理分别是：
置起CLONE_VM标志：
#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sched.h>
 
static int fork_child(void *arg)
{
    int a = (int)arg;
    int i;
    pid_t pid;
 
    printf("In the container, my pid is: %d\n", getpid());
    for (i = 0; i <a; i++) {
        pid = fork();
        if (pid <0)
            return pid;
        else if (pid)
            printf("pid of my child is %d\n", pid);
        else if (pid == 0) {
            sleep(3);
            exit(0);
        }
    }
    
    return 0;
}
 
int main(int argc, char *argv[])
{
    int cpid;
    void *childstack, *stack;
    int flags;
    int ret = 0;
    int stacksize = getpagesize() * 4;
 
    if (argc != 2) {
        fprintf(stderr, "Wrong usage.\n");
        return -1;
    }
 
    stack = malloc(stacksize);
    if (!stack) {
        perror("malloc");
        return -1;
    }
 
    printf("Out of the container, my pid is: %d\n", getpid());
 
    childstack = stack + stacksize;
    //flags = CLONE_NEWPID | CLONE_NEWNS;
    flags = CLONE_NEWPID;
	//flags = 0;
	
    cpid = clone(fork_child, childstack, flags, (void *)atoi(argv[1]));
    printf("cpid: %d\n", cpid);
 
    if (cpid <0) {
        perror("clone");
        ret = -1;
        goto out;
    }
 
    fprintf(stderr, "Parent sleeping 20 seconds\n");
    sleep(20);
    ret = 0;
 
out:
    free(stack);
    return ret;
}
