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

static int count = 0;

void *test_func(void *arg)
{
	int i=0;
	
	for(i=0;i<20000;++i){
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
	pthread_t id[20];
	int i = 0;

	for(i=0;i<20;++i){
		pthread_create(&id[i],NULL,test_func,NULL);
	}

	for(i=0;i<20;++i){
		pthread_join(id[i],NULL);
	}

	printf("%d\n",count);
	
	return 0;
}
