/*
 * test_ptrace_p.c
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
/*
 * @note :
 * 		调用PTRACE_TRACEME并调用execl()后，wait被唤醒， 调用PTRACE_SINGLESTEP(子进程执行一条指令后暂停)后, wait被唤醒 
 * 
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <syscall.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main(void)
{
	long long counter = 0;  /*  machine instruction counter */
	int wait_val;           /*  child's return value        */
	int pid;                /*  child's process id          */

	puts("Please wait");

	switch (pid = fork()) {
	case -1:
		perror("fork");
		break;
	case 0: /*  child process starts        */
		printf("c1\n");
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		printf("c2\n");
		/* 
		 *  must be called in order to allow the
		 *  control over the child process
		 */ 
		execl("/bin/ls", "ls", NULL);
		printf("c3\n");
		/*
		 *  executes the program and causes
		 *  the child to stop and send a signal 
		 *  to the parent, the parent can now
		 *  switch to PTRACE_SINGLESTEP   
		 */ 
		break;
		/*  child process ends  */
	default:/*  parent process starts       */
		wait(&wait_val); 
		printf("p1\n");
		/*   
		 *   parent waits for child to stop at next 
		 *   instruction (execl()) 
		 */
		while (wait_val == 1407 ) {
			counter++;
			if (ptrace(PTRACE_SINGLESTEP, pid, 0, 0) != 0)
				perror("ptrace");
			/* 
			 *   switch to singlestep tracing and 
			 *   release child
			 *   if unable call error.
			 */
			wait(&wait_val);
			/*   wait for next instruction to complete  */
		}
		/*
		 * continue to stop, wait and release until
		 * the child is finished; wait_val != 1407
		 * Low=0177L and High=05 (SIGTRAP)
		 */
	}
	printf("Number of machine instructions : %lld\n", counter);
	return 0;
}
