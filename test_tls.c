/**
 * @filename :
 * 		test_tls.c
 * 
 * @note : 
 * 		(1).如果退出线程是清空了key, 那么key的callback函数也不会去调用；
 * 		(2).PR_SET_NAME之后，可以在ubuntu下通过top -H -p <pid>, 查看线程的运行情况；
 * 		(3).可参考pulseaudio对thread的封装；
 * 
 **/
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <pthread.h>

#ifndef __linux__
#define __linux__
#endif 

#ifdef __linux__
#include <sys/prctl.h>
#endif

static pthread_once_t g_once = PTHREAD_ONCE_INIT;
static pthread_key_t g_key = 0;

typedef struct {	
	pthread_t id;		
	char *name;	
	int flags;
}thread_info_t;

static void free_cb(void *p) {
	thread_info_t *th = (thread_info_t *)p;
	
	printf("%s, %s\n", __func__, th->name);
}

/*
 * @note :
 * 		once_init_routine will be called only one time for current process.
 */
static void once_init_routine(void) {
	printf("%s, 0x%x\n", __func__, (unsigned int)pthread_self());
	
	pthread_key_create(&g_key, free_cb);
}

static void test_tls_destructor(void) __attribute__ ((destructor));
static void test_tls_destructor(void) {
	static void (*_free_cb)(void*) = free_cb;
	
	printf("%s\n", __func__);
	if (_free_cb) { 
		void *p = pthread_getspecific(g_key);
		if (p) {
			_free_cb(p);
		}
	}
	
	pthread_key_delete(g_key);
}

static void *thread_cb(void *pdata) {
	thread_info_t *th = (thread_info_t *)pdata;
	thread_info_t *t;
	int i;
	
	printf("enter %s, 0x%x\n", th->name, (unsigned int)th->id);
	
#ifdef __linux__	
	prctl(PR_SET_NAME, th->name);
#endif	
	
	pthread_once(&g_once, once_init_routine); 
	pthread_setspecific(g_key, th);	
	
	i = 20;
	while (i-- > 0) {
		sleep(1);
	}	
	
	t = pthread_getspecific(g_key);
	if (t) {
		printf("==== %s running ====\n", t->name);
	}
		
	if (th->flags) {
		/* if set to NULL, the callback will not be called. */
		pthread_setspecific(g_key, NULL);
	}
	
	printf("exit %s\n", th->name);
	
	return NULL;
}

int main(int argc, char *argv[]) {
	int ret;	
	thread_info_t thread1;
	thread_info_t thread2;
	
	printf("pid %d\n", getpid());

	memset(&thread1, 0, sizeof(thread1));
	thread1.flags = 1;			//clear pthread key value
	thread1.name = strdup("thread1");
	ret = pthread_create(&thread1.id, NULL, thread_cb, &thread1);
	if (ret != 0) {
		fprintf(stderr, "error create thread1\n");
		return -1;
	}

	memset(&thread2, 0, sizeof(thread2));
	thread2.name = strdup("thread2");
	ret = pthread_create(&thread2.id, NULL, thread_cb, &thread2);
	if (ret != 0) {
		fprintf(stderr, "error create thread2\n");
		pthread_join(thread1.id, NULL);
		if (thread1.name) {
			free(thread1.name);
			thread1.name = NULL;
		}		
		return -1;
	}
	
	pthread_join(thread1.id, NULL);
	pthread_join(thread2.id, NULL);
	
	if (thread1.name) {
		free(thread1.name);
		thread1.name = NULL;
	}

	if (thread2.name) {
		free(thread2.name);
		thread2.name = NULL;
	}
		
	return 0;
}
