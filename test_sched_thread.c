/*
 * @filename :
 * 	test_sched_thread.c
 * @note :
 * 	Demonstrate dynamic scheduling policy use.
 * 	
 1). sudo ./bin/test_sched_thread
  (1). SCHED_FIFO: 先进先出 调度策略允许一个线程运行直到有更高优先级的线程准备好，或者直到它自愿阻塞自己(比如调用pthread_yield之类的函数)。
	在该调度策略下，当有一个更高优先级的线程准备好时，它将立即开始执行。
	此外SCHED_FIFO是一个非分时的简单调度策略，当一个线程变成可运行状态，它将被追加到对应优先级队列的尾部((POSIX 1003.1)。
	当所有高优先级的线程终止或者阻塞时，它将被运行。对于相同优先级别的线程，按照简单的先进先运行的规则运行。我们考虑一种很坏的情况，
	如果有若干相同优先级的线程等待执行，然而最早执行的线程无终止或者阻塞动作，那么其他线程是无法执行的，除非当前线程调用如pthread_yield之类的函数，
	所以在使用SCHED_FIFO的时候要小心处理相同级别线程的动作。

  (2). SCHED_RR: 轮循调度策略，除了考虑优先级之外，还加入了时间片的限制。当一个线程执行完了一个时间片，
	并且有其它的SCHED_RR或者SCHED_FIFO调度策略的相同优先级的线程准备好时，运行线程将被准备好的线程抢占。

	鉴于SCHED_FIFO调度策略的一些缺点，SCHED_RR对SCHED_FIFO做出了一些增强功能。从实质上看，它还是SCHED_FIFO调用策略。
	它使用最大运行时间来限制当前进程的运行，当运行时间大于等于最大运行时间的时候，当前线程将被切换并放置于相同优先级队列的最后。
	这样做的好处是其他具有相同级别的线程能在“自私“线程下执行。（百度百科）

  (3). SCHED_OTHER: 是Linux默认的分时调度策略。
	它是默认的线程分时调度策略，所有的线程的优先级别都是0，线程的调度是通过分时来完成的。简单地说，如果系统使用这种调度策略，程序将无法设置线程的优先级
	
 2). priority越大，获得的运行时间越多 (SCHED_RR)
 
 * 
 */
#include <stdio.h> 
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
//#include "errors.h"

#define THREADS 5

#define TEST_NORMAL

#define TEST_PRIORITY_OFFSET	10

#define TEST_SCHED_POLICY	SCHED_RR

typedef struct thread_tag {
	int index;
	pthread_t id;
} thread_t;

thread_t threads[THREADS];
int min_priority;
int max_priority;

void *thread_routine(void *arg)
{
	thread_t *self = (thread_t *)arg;
	struct sched_param my_param;
	int my_policy;
	int status;
	int i = 0;
	int j = 0;
	
#ifdef TEST_NORMAL
	my_param.sched_priority = min_priority + self->index;
#else	
	my_param.sched_priority = min_priority + TEST_PRIORITY_OFFSET - self->index;
#endif	
	if(pthread_setschedparam(self->id, TEST_SCHED_POLICY, &my_param) != 0)
		printf("pthread_setschedparam failed\n");
	pthread_getschedparam(self->id, &my_policy, &my_param);
	printf("thread_routine %d running at %s/%d\n",
		self->index,
		(my_policy == SCHED_FIFO ? "FIFO"
			: (my_policy == SCHED_RR ? "RR"
			: (my_policy == SCHED_OTHER ? "OTHER"
			: "unknown"))),
		my_param.sched_priority);
	while (i++ < 1000000) {
		j = 0;
		while (j++ < 100);
	}		
	printf("exit %d\n", self->index);

	return NULL;
}

int main(int argc, char *argv[])
{
	int count;

	min_priority = sched_get_priority_min(TEST_SCHED_POLICY);
	max_priority = sched_get_priority_max(TEST_SCHED_POLICY);
	if(min_priority == -1 || max_priority == -1){
		//errno_abort("Get SCHED_RR min priority");
		_exit(-1);
	}
	printf("priority : min = %d, max = %d\n", min_priority, max_priority);
	for(count = 0; count < THREADS; count++){
		threads[count].index = count;
		pthread_create(&threads[count].id, NULL,
				 thread_routine, (void *)&threads[count]);
	}
	for(count = 0; count < THREADS; count++){
		pthread_join(threads[count].id, NULL);
	}
	printf("Main exiting\n");

	return 0;
}
