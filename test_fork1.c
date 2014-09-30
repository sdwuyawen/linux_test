/*
 *	@filename :
 * 		test_fork1.c 
 *
 * 	@note :
 * 	  1.sample1输出8个- 
 *     1). fork()系统调用是Unix下以自身进程创建子进程的系统调用，一次调用，两次返回，如果返回是0，则是子进程，如果返回值>0，则是父进程（返回值是子进程的pid），这是众为周知的。
 * 		         -> fu2
 * 		  -> fu1 -> zi2
 * 		fu 
 *        -> zi1 -> fu3
 * 		         -> zi3
 * 	   2). 还有一个很重要的东西是，在fork()的调用处，整个父进程空间会原模原样地复制到子进程中，包括指令，变量值，程序调用栈，环境变量，缓冲区，等等。
 * 		所以，上面的那个程序为什么会输入8个“-”，这是因为printf(“-“);语句有buffer，所以，对于上述程序，printf(“-“);把“-”放到了缓存中，
 * 		并没有真正的输出（参看《C语言的迷题》中的第一题），在fork的时候，缓存被复制到了子进程空间，所以，就多了两个，就成了8个，而不是6个。
 * 		stdout是块设备，stderr则不是。对于块设备，只有当下面几种情况下才会被输入，1）遇到回车，2）缓冲区满，3）flush被调用。而stderr则不会。
 * 
 *    2.sample2输出6个-
 * 
 * 	  3.sampel3输出：
 *      $./bin/test_fork1
 *		ppid=14681, pid=16079, i=0 
 *		ppid=16079, pid=16080, i=0 
 *		ppid=14681, pid=16079, i=1 
 *		ppid=16079, pid=16080, i=1 
 *		ppid=16079, pid=16081, i=1 
 *		ppid=16080, pid=16082, i=1 
 * 
 *		$pstree -p | grep test_fork1
 *      |                      |-bash(14681)---test_fork1(16079)-+-test_fork1(16080)---test_fork1(16082)
 *      |                      |                                 `-test_fork1(16081)
 * 
 * 	  4.sample输出8次， ppid=14681, pid=16412, i=0 ->和ppid=16412, pid=16413, i=0 -> 重复输出一次
 *		ppid=14681, pid=16412, i=0 -> ppid=14681, pid=16412, i=1 -> ppid=16412, pid=16413, i=0 -> ppid=16412, pid=16413, i=1 -> ppid=14681, pid=16412, i=0 -> ppid=16412, pid=16414, i=1 -> ppid=16412, pid=16413, i=0 -> ppid=16413, pid=16415, i=1 ->
 */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
 
int main(void)
{
   int i;
   for(i=0; i<2; i++){
      fork();
      //printf("-");		//sample 1
      //printf("-\n");		//sample 2
      //printf("ppid=%d, pid=%d, i=%d\n", getppid(), getpid(), i);	//sample 3
      printf("ppid=%d, pid=%d, i=%d -> ", getppid(), getpid(), i);	//sample 4
   }
   sleep(10); //让进程停留十秒，这样我们可以用pstree查看一下进程树
   
   return 0;
}
