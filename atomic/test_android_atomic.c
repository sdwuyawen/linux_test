/**
 * @filename :
 * 	test_android_atomic.c
 * 
 * @note :
 * 
 **/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "atomic.h"

#define TEST_THREADS	20

static int count = 0;
static int32_t flags = 0;

void *test_func(void *arg)
{
	int i=0;
	
	for(i=0; i<20000; ++i){
	#if 1
		android_atomic_add(1, &count);
	#else
		count += 1;
	#endif
	}
	
	return NULL;
}

int main(int argc, const char *argv[])
{
	pthread_t id[TEST_THREADS];
	int i = 0;
	int32_t ret;
	
	for(i=0; i<TEST_THREADS; ++i){
		pthread_create(&id[i],NULL,test_func,NULL);
	}

	for(i=0; i<TEST_THREADS; ++i){
		pthread_join(id[i],NULL);
	}

	printf("%d\n",count);
	printf("flags = 0x%x\n", flags);
	ret = android_atomic_or(0x01, &flags);
	printf("android_atomic_or 0x01 flags = 0x%x, ret = 0x%x\n", flags, ret);
	ret = android_atomic_and(~0x01, &flags);
	printf("android_atomic_and ~0x01 flags = 0x%x, ret = 0x%x\n", flags, ret);
	
	return 0;
}
