/**
 * 	@filename :
 * 		test_mem_bar.cpp
 * 
 *  @note :
 * 	1).与进程的情况相似，线程亲和性的设置和获取主要通过下面两个函数来实现：
    int pthread_setaffinity_np(pthread_t thread, size_t cpusetsize，const cpu_set_t *cpuset);
    int pthread_getaffinity_np(pthread_t thread, size_t cpusetsize, cpu_set_t *cpuset);
    从函数名以及参数名都很明了，唯一需要点解释下的可能就是cpu_set_t这个结构体了。这个结构体的理解类似于select中的fd_set，可以理解为cpu集，
    也是通过约定好的宏来进行清除、设置以及判断：
	   //初始化，设为空
      void CPU_ZERO (cpu_set_t *set); 
      //将某个cpu加入cpu集中 
       void CPU_SET (int cpu, cpu_set_t *set); 
       //将某个cpu从cpu集中移出 
       void CPU_CLR (int cpu, cpu_set_t *set); 
       //判断某个cpu是否已在cpu集中设置了 
       int CPU_ISSET (int cpu, const cpu_set_t *set); 
    两个线程分别分配到CPU0, CPU1
	只要内存不出现乱序访问，那么 r1 和 r2 不可能同时为 0，因此断言失败表示存在内存乱序访问。编译之后运行此程序，会发现存在一定概率导致断言失败
   2). 如果把cpu_thread2改为0, 就是说两个线程都分配到CPU0, 那么就不会出现导致断言失败
   3). 在run函数中加入memory barrier, 就不会出现导致断言失败
    (1).
    #ifdef CONFIG_SMP
    #define smp_mb() mb()
    #else
    #define smp_mb() barrier()
    #endif
    (2).
    Memory barrier 常用场合包括：
		实现同步原语（synchronization primitives）
		实现无锁数据结构（lock-free data structures）
		驱动程序
   
 **/
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
 
// -------------------
int cpu_thread1 = 0;
int cpu_thread2 = 1;
//int cpu_thread2 = 0;	//和thread1同一个cpu
 
volatile int x, y, r1, r2;
 
void start()
{
	x = y = r1 = r2 = 0;
}
 
void end()
{
	printf("%d,%d\n", r1, r2);
	assert(!(r1 == 0 && r2 == 0));
}
 
void run1()
{
	x = 1;
	__asm__ __volatile__("mfence" ::: "memory");
	r1 = y;
}
 
void run2()
{
	y = 1;
	__asm__ __volatile__("mfence" ::: "memory");
	r2 = x;
}
 
// -------------------
static pthread_barrier_t barrier_start;
static pthread_barrier_t barrier_end;
 
static void* thread1(void*)
{
	while (1) {
		pthread_barrier_wait(&barrier_start);
		run1();
		pthread_barrier_wait(&barrier_end);
	}
 
	return NULL;
}
 
static void* thread2(void*)
{
	while (1) {
		pthread_barrier_wait(&barrier_start);
		run2();
		pthread_barrier_wait(&barrier_end);
	}
 
	return NULL;
}
 
int main()
{
	assert(pthread_barrier_init(&barrier_start, NULL, 3) == 0);
	assert(pthread_barrier_init(&barrier_end, NULL, 3) == 0);
 
	pthread_t t1;
	pthread_t t2;
	assert(pthread_create(&t1, NULL, thread1, NULL) == 0);
	assert(pthread_create(&t2, NULL, thread2, NULL) == 0);
 
	cpu_set_t cs;
	CPU_ZERO(&cs);
	CPU_SET(cpu_thread1, &cs);
	assert(pthread_setaffinity_np(t1, sizeof(cs), &cs) == 0);
	CPU_ZERO(&cs);
	CPU_SET(cpu_thread2, &cs);
	assert(pthread_setaffinity_np(t2, sizeof(cs), &cs) == 0);
 
	while (1) {
		start();
		pthread_barrier_wait(&barrier_start);
		pthread_barrier_wait(&barrier_end);
		end();
	}
 
	return 0;
}
