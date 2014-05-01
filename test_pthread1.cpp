/**
   @filename :
     test_pthread1.cpp
	
   @note :
     test pthread_once
	 在LinuxThreads中，实际"一次性函数"的执行状态有三种：NEVER（0）、IN_PROGRESS（1）、DONE （2），
	 如果once初值设为1，则由于所有pthread_once()都必须等待其中一个激发"已执行一次"信号，因此所有pthread_once ()都会陷入永久的等待中；
	 如果设为2，则表示该函数已执行过一次，从而所有pthread_once()都会立即返回0。
**/
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

static pthread_once_t once = PTHREAD_ONCE_INIT;

void once_run(void)
{
	 cout<<"once_run in thread "<<(unsigned int )pthread_self()<<endl;
}

void * child1(void * arg)
{
	 int ret;
	 
	 pthread_t tid = pthread_self();
	 cout<<"thread "<<(unsigned int )tid<<" enter"<<endl;
	 ret = pthread_once(&once, once_run);
	 cout<<"thread "<<tid<<" return once= "<<once<<endl;
}


void * child2(void * arg)
{
	 int ret;
	 
	 pthread_t tid =pthread_self();
	 cout<<"thread "<<(unsigned int )tid<<" enter"<<endl;
	 ret = pthread_once(&once,once_run);
	 cout<<"thread "<<tid<<" return once= "<<once<<endl;
}

int main(void)
{
	 pthread_t tid1,tid2;
	 cout<<"hello -> once= "<<once<<endl;
	 pthread_create(&tid1,NULL,child1,NULL);
	 sleep(1);
	 pthread_create(&tid2,NULL,child2,NULL);
	 sleep(1);
	 cout<<"main thread exit"<<endl;
	 return 0;

}
