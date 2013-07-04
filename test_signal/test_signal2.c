/**
 * 	@filename :
 * 		test_signal2.c
 * 
 * 	@note :
 * 		(1).
 * 		在Linux中使用longjmp有一个问题，当捕捉到一个信号的时候进入信号处理函数，此时当前信号自动加入到
 *		进程的信号屏蔽字中，这就阻止了后来的这种信号中断该信号处理程序。如果用longjmp跳出信号处理程序，
 * 		那么该进程的信号屏蔽字不会被恢复到调用信号处理程序前的信号屏蔽字。
 *		如果想恢复的话，就要使用sigsetjmp和siglongjmp函数。
 **/
 /*
  (1).int sigprocmask(int how, const sigset_t *restrict set, sigset_t *restrict oldset);
  一个进程的信号屏蔽字规定了当前阻塞而不能递送给该进程的信号集。sigprocmask()可以用来检测或改变目前的信号屏蔽字，其操作依参数how来决定，如果参数oldset不是NULL指针，那么目前的信号屏蔽字会由此指针返回。如果set是一个非空指针，则参数how指示如何修改当前信号屏蔽字。每个进程都有一个用来描述哪些信号递送到进程时将被阻塞的信号集，该信号集中的所有信号在递送到进程后都将被阻塞。
	参数how：
		SIG_BLOCK
		该进程新的信号屏蔽字是其当前信号屏蔽字和set指向信号集的并集。set包含了我
		们希望阻塞的附加信号。
		
		SIG_UNBLOCK	
		该进程新的信号屏蔽字是其当前信号屏蔽字和set所指向信号集的补集的交集。set包含了
		我们希望解除阻塞的信号.
		
		SIG_SETMASK		
		该进程新的信号屏蔽是set指向的值
		除SIG_SETMASK外,如果set是个空指针，则不改变该进程的信号屏蔽字，how的值也无意义。
		SIG_SETMASK与set空指针结合使用，即清空所有屏蔽的信号。
		
	返回值
		执行成功返回0，失败返回-1。
		
	错误代码
		EFAULT
		参数set，oldset指针地址无法存取
		
		EINTR	
		此调用被中断	
	
	
	(2).int sigismember(const sigset_t *set,int signum);
	
	sigemptyset，sigfillset，sigaddset，sigdelset
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <errno.h>

//#define SIG_JMP_EN

#ifdef SIG_JMP_EN
static sigjmp_buf     jmpbuf;
#else
static jmp_buf        jmpbuf1;
#endif

static void sig_usr1(int signo);
static void pr_mask(char* ptr);

int main(int argc, char** argv)
{
	sigset_t                set;
	struct sigaction        act, oact;
	
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	if (sigprocmask(SIG_SETMASK, &set, NULL) != 0) {
		perror("sigparocmask error");
		exit(1);
	}
	
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	act.sa_handler = sig_usr1;
	if (sigaction(SIGUSR1, &act, &oact) != 0) {
		perror("sigaction error");
		exit(2);
	}
	
	pr_mask("before main: ");
#ifdef SIG_JMP_EN
	if(sigsetjmp(jmpbuf, 1)){
		pr_mask("end main: ");
	}
#else
	if (setjmp(jmpbuf1)) {
		pr_mask("end main: ");
	}
#endif	

	pause();
	
	exit(0);
}

static void sig_usr1(int signo)
{
	pr_mask("int sig_int: ");
#ifdef SIG_JMP_EN	
	siglongjmp(jmpbuf, 1);
#else	
	longjmp(jmpbuf1, 1);
#endif	
}

static void pr_mask(char* ptr)
{
	sigset_t                sigset;
	int                     errno_save;
	int                     ret;
	
	errno_save = errno;
	ret = sigprocmask(0, NULL, &sigset);		//获取当前信号屏蔽
	if (ret == -1) {
		perror("sigprocmask error");
		exit(1);
	}
	
	printf("%s", ptr);
	if (sigismember(&sigset, SIGINT)) {			//sigset是否包含位SIGINT
		printf("SIGINT ");
	}
	if (sigismember(&sigset, SIGQUIT)) {
		printf("SIGQUIT ");
	}
	if (sigismember(&sigset, SIGUSR1)) {
		printf("SIGUSR1 ");
	}
	if (sigismember(&sigset, SIGALRM)) {
		printf("SIGALRM ");
	}
	printf("\n");
	
	errno = errno_save;
}

