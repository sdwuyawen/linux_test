/*
 * test_ptrace_t.c
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
 * 	@note :
 * 		./test_ptrace_t <thread_id>
 * 
 */ 
#include <sys/ptrace.h>
#include <stdlib.h>     // for atoi
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h> /* For user_regs_struct */

int main(int argc, char *argv[])
{ 
    pid_t traced_process;
    struct user_regs_struct regs;
    long ins;

    if (argc != 2) {
        printf("Usage: %s <pid to be traced> ",
               argv[0], argv[1]);
        exit(1);
    }   
    traced_process = atoi(argv[1]);
    ptrace(PTRACE_ATTACH, traced_process, 
           NULL, NULL);
    wait(NULL);
    ptrace(PTRACE_GETREGS, traced_process, 
           NULL, &regs);
#if __WORDSIZE == 64           
    ins = ptrace(PTRACE_PEEKTEXT, traced_process, 
                 regs.rip, NULL);
    printf("RIP: %lx Instruction executed: %lx \n", 
           regs.rip, ins);
#else
    ins = ptrace(PTRACE_PEEKTEXT, traced_process, 
                 regs.eip, NULL);
    printf("EIP: %lx Instruction executed: %lx \n", 
           regs.eip, ins);
#endif                 
    ptrace(PTRACE_DETACH, traced_process, 
           NULL, NULL);

    return 0;
}
