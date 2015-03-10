/*
 * 	@filename :
 * 		test_relock.c
 *
 * 	@note :
 * 		1). mutex默认不支持重入， 需要设定PTHREAD_MUTEX_RECURSIVE属性
 * 		2). 设定PTHREAD_MUTEX_RECURSIVE属性后，调用了pthread_mutex_lock函数n次，同样
 * 		也要调用pthread_mutex_unlock函数n次才能释放锁.
 */
#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

static pthread_mutex_t g_mutex;
static pthread_mutexattr_t g_attr;
static pthread_t g_thread1;

static void hold_mutex(int c)
{
	int ret;
	
	if (c == 0) {
		return;
	}


	printf("enter hold_mutex [caller %d]\n", c);

	ret = pthread_mutex_lock(&g_mutex);
	
	hold_mutex(c-1);

	sleep(1);
	pthread_mutex_unlock(&g_mutex);
	printf("leave hold_mutex [caller %d]\n", c);
}

static void *thread_callback(void *pdata) 
{
	hold_mutex(5);
	return NULL;
}

int main()
{
    pthread_mutexattr_init(&g_attr);
    pthread_mutexattr_settype(&g_attr, PTHREAD_MUTEX_RECURSIVE);	
	pthread_mutex_init(&g_mutex, &g_attr);
	pthread_create(&g_thread1, NULL, thread_callback, NULL);	
	sleep(1);
	pthread_mutex_lock(&g_mutex);
	printf("test\n");
	pthread_mutex_unlock(&g_mutex);
	pthread_join(g_thread1, NULL);
	pthread_mutex_destroy(&g_mutex);
	
	return 0;
} 
