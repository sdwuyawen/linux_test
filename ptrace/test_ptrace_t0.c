/*
 * test_ptrace_t0.c
 * 
 * Copyright 2014 Paul <wu_bao@163.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
#include <stdio.h>
#include <unistd.h> //for sleep
#include <stdlib.h> //for exit
#include <pthread.h>//for pthread
#include <errno.h>  //for errno
#include <sys/syscall.h> //for gettid

#define gettid() syscall(__NR_gettid)

void *func(void *para)
{
    printf("Hello world.\n");
    printf("child process tid: %u\n", gettid());
    sleep(-1);	// 该进程一直sleep，等待
    return NULL;
}

int main(int argc, char *argv[])
{ 
    pthread_t tid;
    
    int ret = pthread_create(&tid, NULL, func, NULL);
    if (ret != 0) {   
        exit(errno);
    }   
    printf("parent process pid: %u\n", getpid());

    pthread_join(tid, NULL);

    return 0;
}
