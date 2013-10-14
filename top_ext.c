/*
 * Copyright (c) 2008, The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the 
 *    distribution.
 *  * Neither the name of Google, Inc. nor the names of its contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <ctype.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include <sched.h>

struct cpu_info {
    long unsigned utime, ntime, stime, itime;
    long unsigned iowtime, irqtime, sirqtime;
};

#define FILE_NAME_MAX	128
#define FILE_BUF_SIZE	256

#define PROC_NAME_LEN 64
#define THREAD_NAME_LEN 32
#define POLICY_NAME_LEN 4

struct proc_info {
    struct proc_info *next;
    pid_t pid;
    pid_t tid;
    uid_t uid;
    gid_t gid;
    char name[PROC_NAME_LEN];
    char tname[THREAD_NAME_LEN];
    char state;
    long unsigned utime;
    long unsigned stime;
    long unsigned delta_utime;
    long unsigned delta_stime;
    long unsigned delta_time;
    long vss;
    long rss;
    int prs;
    int num_threads;
    char policy[POLICY_NAME_LEN];    
};

struct proc_list {
    struct proc_info **array;
    int size;
};

#define die(...) { fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE); }

#define INIT_PROCS 50
#define THREAD_MULT 8
static struct proc_info **old_procs, **new_procs;
static int num_old_procs, num_new_procs;
static struct proc_info *free_procs;
static int num_used_procs, num_free_procs;

static int max_procs, delay, iterations, threads;

static int num_def_pids;
static pid_t def_pids[INIT_PROCS];

static int is_show_files, is_show_stack;

static struct cpu_info old_cpu, new_cpu;

static struct proc_info *alloc_proc(void);
static void free_proc(struct proc_info *proc);
static void read_procs(void);
static int read_stat(char *filename, struct proc_info *proc);
static void read_policy(int pid, struct proc_info *proc);
static void add_proc(int proc_num, struct proc_info *proc);
static int read_cmdline(char *filename, struct proc_info *proc);
static int read_status(char *filename, struct proc_info *proc);
static void print_procs(void);
static struct proc_info *find_old_proc(pid_t pid, pid_t tid);
static void free_old_procs(void);
static int (*proc_cmp)(const void *a, const void *b);
static int proc_cpu_cmp(const void *a, const void *b);
static int proc_vss_cmp(const void *a, const void *b);
static int proc_rss_cmp(const void *a, const void *b);
static int proc_thr_cmp(const void *a, const void *b);
static int numcmp(long long a, long long b);
static void usage(char *cmd);

#if 0
int top_main(int argc, char *argv[]) {
#else
int main(int argc, char *argv[]) {
#endif	
    int i;

    num_used_procs = num_free_procs = 0;
	num_def_pids = 0;
	
    max_procs = 0;
    delay = 3;
    iterations = -1;
    proc_cmp = &proc_cpu_cmp;
    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-p")) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Option -p expects an argument.\n");
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            
            ++i;
            for (; i < argc; ) {            
				//printf("pid %s\n", argv[i]);
				if (argv[i][0] != '-') {
					if (num_def_pids < INIT_PROCS) {
						def_pids[num_def_pids++] = atoi(argv[i]);
					}
					i++;
				} else {
					i--;
					break;
				}
			}
            continue;
        }
        
        if (!strcmp(argv[i], "--file")) {
			is_show_files = 1;
			continue;
		}

        if (!strcmp(argv[i], "--stack")) {
			is_show_stack = 1;
			continue;
		}
		
        if (!strcmp(argv[i], "-m")) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Option -m expects an argument.\n");
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            max_procs = atoi(argv[++i]);
            continue;
        }
        if (!strcmp(argv[i], "-n")) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Option -n expects an argument.\n");
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            iterations = atoi(argv[++i]);
            continue;
        }
        if (!strcmp(argv[i], "-d")) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Option -d expects an argument.\n");
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            delay = atoi(argv[++i]);
            continue;
        }
        if (!strcmp(argv[i], "-s")) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Option -s expects an argument.\n");
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            ++i;
            if (!strcmp(argv[i], "cpu")) { proc_cmp = &proc_cpu_cmp; continue; }
            if (!strcmp(argv[i], "vss")) { proc_cmp = &proc_vss_cmp; continue; }
            if (!strcmp(argv[i], "rss")) { proc_cmp = &proc_rss_cmp; continue; }
            if (!strcmp(argv[i], "thr")) { proc_cmp = &proc_thr_cmp; continue; }
            fprintf(stderr, "Invalid argument \"%s\" for option -s.\n", argv[i]);
            exit(EXIT_FAILURE);
        }
        if (!strcmp(argv[i], "-t")) { threads = 1; continue; }
        if (!strcmp(argv[i], "-h")) {
            usage(argv[0]);
            exit(EXIT_SUCCESS);
        }
        fprintf(stderr, "Invalid argument \"%s\".\n", argv[i]);
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    if (threads && proc_cmp == &proc_thr_cmp) {
        fprintf(stderr, "Sorting by threads per thread makes no sense!\n");
        exit(EXIT_FAILURE);
    }

    free_procs = NULL;

    num_new_procs = num_old_procs = 0;
    new_procs = old_procs = NULL;

    read_procs();
    while ((iterations == -1) || (iterations-- > 0)) {
        old_procs = new_procs;
        num_old_procs = num_new_procs;
        memcpy(&old_cpu, &new_cpu, sizeof(old_cpu));
        sleep(delay);
        read_procs();
        print_procs();
        free_old_procs();
    }

    return 0;
}

static struct proc_info *alloc_proc(void) {
    struct proc_info *proc;

    if (free_procs) {
        proc = free_procs;
        free_procs = free_procs->next;
        num_free_procs--;
    } else {
        proc = malloc(sizeof(*proc));
        if (!proc) die("Could not allocate struct process_info.\n");
    }

    num_used_procs++;

    return proc;
}

static void free_proc(struct proc_info *proc) {
    proc->next = free_procs;
    free_procs = proc;

    num_used_procs--;
    num_free_procs++;
}

#define MAX_LINE 256

static void read_procs(void) {
    DIR *proc_dir, *task_dir;
    struct dirent *pid_dir, *tid_dir;
    char filename[64];
    FILE *file;
    int proc_num;
    struct proc_info *proc;
    pid_t pid, tid;

    int i;

    proc_dir = opendir("/proc");
    if (!proc_dir) die("Could not open /proc.\n");

    new_procs = calloc(INIT_PROCS * (threads ? THREAD_MULT : 1), sizeof(struct proc_info *));
    num_new_procs = INIT_PROCS * (threads ? THREAD_MULT : 1);

    file = fopen("/proc/stat", "r");
    if (!file) die("Could not open /proc/stat.\n");
    fscanf(file, "cpu  %lu %lu %lu %lu %lu %lu %lu", &new_cpu.utime, &new_cpu.ntime, &new_cpu.stime,
            &new_cpu.itime, &new_cpu.iowtime, &new_cpu.irqtime, &new_cpu.sirqtime);
    fclose(file);

    proc_num = 0;
    while ((pid_dir = readdir(proc_dir))) {
        if (!isdigit(pid_dir->d_name[0]))
            continue;

        pid = atoi(pid_dir->d_name);
        
        if (num_def_pids) {
			for (i=0; i<num_def_pids; i++) {
				if (pid == def_pids[i]) {
					break;
				}
			}
			
			if (i == num_def_pids) {
				continue;
			}
		}
        
        struct proc_info cur_proc;
        
        if (!threads) {
            proc = alloc_proc();

            proc->pid = proc->tid = pid;

            sprintf(filename, "/proc/%d/stat", pid);
            read_stat(filename, proc);

            sprintf(filename, "/proc/%d/cmdline", pid);
            read_cmdline(filename, proc);

            sprintf(filename, "/proc/%d/status", pid);
            read_status(filename, proc);

            read_policy(pid, proc);

            proc->num_threads = 0;
        } else {
            sprintf(filename, "/proc/%d/cmdline", pid);
            read_cmdline(filename, &cur_proc);

            sprintf(filename, "/proc/%d/status", pid);
            read_status(filename, &cur_proc);
            
            proc = NULL;
        }

        sprintf(filename, "/proc/%d/task", pid);
        task_dir = opendir(filename);
        if (!task_dir) continue;

        while ((tid_dir = readdir(task_dir))) {
            if (!isdigit(tid_dir->d_name[0]))
                continue;

            if (threads) {
                tid = atoi(tid_dir->d_name);

                proc = alloc_proc();

                proc->pid = pid; proc->tid = tid;

                sprintf(filename, "/proc/%d/task/%d/stat", pid, tid);
                read_stat(filename, proc);

                read_policy(tid, proc);

                strcpy(proc->name, cur_proc.name);
                proc->uid = cur_proc.uid;
                proc->gid = cur_proc.gid;

                add_proc(proc_num++, proc);
            } else {
                proc->num_threads++;
            }
        }

        closedir(task_dir);
        
        if (!threads)
            add_proc(proc_num++, proc);
    }

    for (i = proc_num; i < num_new_procs; i++)
        new_procs[i] = NULL;

    closedir(proc_dir);
}

static int read_stat(char *filename, struct proc_info *proc) {
    FILE *file;
    char buf[MAX_LINE], *open_paren, *close_paren;
    int res, idx;

    file = fopen(filename, "r");
    if (!file) return 1;
    fgets(buf, MAX_LINE, file);
    fclose(file);

    /* Split at first '(' and last ')' to get process name. */
    open_paren = strchr(buf, '(');
    close_paren = strrchr(buf, ')');
    if (!open_paren || !close_paren) return 1;

    *open_paren = *close_paren = '\0';
    strncpy(proc->tname, open_paren + 1, THREAD_NAME_LEN);
    proc->tname[THREAD_NAME_LEN-1] = 0;
    
    /* Scan rest of string. */
    sscanf(close_paren + 1, " %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d "
                 "%lu %lu %*d %*d %*d %*d %*d %*d %*d %lu %ld "
                 "%*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %d",
                 &proc->state, &proc->utime, &proc->stime, &proc->vss, &proc->rss, &proc->prs);

    return 0;
}

static void add_proc(int proc_num, struct proc_info *proc) {
    int i;

    if (proc_num >= num_new_procs) {
        new_procs = realloc(new_procs, 2 * num_new_procs * sizeof(struct proc_info *));
        if (!new_procs) die("Could not expand procs array.\n");
        for (i = num_new_procs; i < 2 * num_new_procs; i++)
            new_procs[i] = NULL;
        num_new_procs = 2 * num_new_procs;
    }
    new_procs[proc_num] = proc;
}

static int read_cmdline(char *filename, struct proc_info *proc) {
    FILE *file;
    char line[MAX_LINE];

    line[0] = '\0';
    file = fopen(filename, "r");
    if (!file) return 1;
    fgets(line, MAX_LINE, file);
    fclose(file);
    if (strlen(line) > 0) {
        strncpy(proc->name, line, PROC_NAME_LEN);
        proc->name[PROC_NAME_LEN-1] = 0;
    } else
        proc->name[0] = 0;
    return 0;
}

#ifndef SCHED_NORMAL
  #define SCHED_NORMAL 0
#endif

#ifndef SCHED_BATCH
  #define SCHED_BATCH 3
#endif

/* Keep in sync with THREAD_GROUP_* in frameworks/base/core/java/android/os/Process.java */
typedef enum {
    SP_DEFAULT    = -1,
    SP_BACKGROUND = 0,
    SP_FOREGROUND = 1,
    SP_SYSTEM     = 2,  // can't be used with set_sched_policy()
    SP_AUDIO_APP  = 3,
    SP_AUDIO_SYS  = 4,
    SP_CNT,
    SP_MAX        = SP_CNT - 1,
    SP_SYSTEM_DEFAULT = SP_FOREGROUND,
} SchedPolicy;

static int get_sched_policy(int tid, SchedPolicy *policy)
{
#ifdef HAVE_GETTID
    if (tid == 0) {
        tid = gettid();
    }
#endif

	{
        int rc = sched_getscheduler(tid);
        if (rc < 0)
            return -1;
        else if (rc == SCHED_NORMAL)
            *policy = SP_FOREGROUND;
        else if (rc == SCHED_BATCH)
            *policy = SP_BACKGROUND;
        else {
            errno = ERANGE;
            return -1;
        }
    }
    return 0;
}

/* Re-map SP_DEFAULT to the system default policy, and leave other values unchanged.
 * Call this any place a SchedPolicy is used as an input parameter.
 * Returns the possibly re-mapped policy.
 */
static inline SchedPolicy _policy(SchedPolicy p)
{
   return p == SP_DEFAULT ? SP_SYSTEM_DEFAULT : p;
}

static const char *get_sched_policy_name(SchedPolicy policy)
{
    policy = _policy(policy);
    static const char * const strings[SP_CNT] = {
       [SP_BACKGROUND] = "bg",
       [SP_FOREGROUND] = "fg",
       [SP_SYSTEM]     = "  ",
       [SP_AUDIO_APP]  = "aa",
       [SP_AUDIO_SYS]  = "as",
    };
    if ((policy < SP_CNT) && (strings[policy] != NULL))
        return strings[policy];
    else
        return "error";
}

static void read_policy(int pid, struct proc_info *proc) {
	#if 1
    SchedPolicy p;
    if (get_sched_policy(pid, &p) < 0)
        strcpy(proc->policy, "unk");
    else {
        strcpy(proc->policy, get_sched_policy_name(p));
        proc->policy[2] = '\0';
    }
    #endif
}

static int read_status(char *filename, struct proc_info *proc) {
    FILE *file;
    char line[MAX_LINE];
    unsigned int uid, gid;

    file = fopen(filename, "r");
    if (!file) return 1;
    while (fgets(line, MAX_LINE, file)) {
        sscanf(line, "Uid: %u", &uid);
        sscanf(line, "Gid: %u", &gid);
    }
    fclose(file);
    proc->uid = uid; proc->gid = gid;
    return 0;
}

static void print_stack(pid_t pid, pid_t tid) {
    char filename[FILE_NAME_MAX];
	FILE *file;
	char buffer[FILE_BUF_SIZE];

	sprintf(filename, "/proc/%d/task/%d/stack", pid, tid);
	
	file = fopen(filename, "r");
	if (file == NULL) {
		return ;
	}
	printf("\033[32;1m");
	while (fgets(buffer, FILE_BUF_SIZE, file)) {
		printf("\t %s", buffer);
	}
	printf("\033[0m\n");
	
	fclose(file);
	
	return ;
}

static void print_files(pid_t pid) {
	DIR *fd_dir;
	struct dirent *file_ent;				
	
    char linkname[FILE_NAME_MAX];
    char filename[FILE_NAME_MAX];
    char dirname[FILE_NAME_MAX];

	sprintf(dirname, "/proc/%d/fd", pid);
	fd_dir = opendir(dirname);				
	if (fd_dir) {
		printf("\033[35;1m");
		while ((file_ent = readdir(fd_dir))) {
			if (!isdigit(file_ent->d_name[0]))
				continue;
			
			sprintf(linkname, "%s/%s", dirname, file_ent->d_name); 
			memset(filename, 0, sizeof(filename));
			readlink(linkname, filename, FILE_NAME_MAX);
			printf("\t %04s -> %s\n", file_ent->d_name, filename);
		}
		printf("\033[0m\n");
		closedir(fd_dir);
	}	
}

static void print_procs(void) {
    int i;
    struct proc_info *old_proc, *proc;
    long unsigned total_delta_time;
    struct passwd *user;
    struct group *group;
    char *user_str, user_buf[20];
    char *group_str, group_buf[20];

    for (i = 0; i < num_new_procs; i++) {
        if (new_procs[i]) {
            old_proc = find_old_proc(new_procs[i]->pid, new_procs[i]->tid);
            if (old_proc) {
                new_procs[i]->delta_utime = new_procs[i]->utime - old_proc->utime;
                new_procs[i]->delta_stime = new_procs[i]->stime - old_proc->stime;
            } else {
                new_procs[i]->delta_utime = 0;
                new_procs[i]->delta_stime = 0;
            }
            new_procs[i]->delta_time = new_procs[i]->delta_utime + new_procs[i]->delta_stime;
        }
    }

    total_delta_time = (new_cpu.utime + new_cpu.ntime + new_cpu.stime + new_cpu.itime
                        + new_cpu.iowtime + new_cpu.irqtime + new_cpu.sirqtime)
                     - (old_cpu.utime + old_cpu.ntime + old_cpu.stime + old_cpu.itime
                        + old_cpu.iowtime + old_cpu.irqtime + old_cpu.sirqtime);

    qsort(new_procs, num_new_procs, sizeof(struct proc_info *), proc_cmp);

    printf("\n\n\n");
    printf("User %ld%%, System %ld%%, IOW %ld%%, IRQ %ld%%\n",
            ((new_cpu.utime + new_cpu.ntime) - (old_cpu.utime + old_cpu.ntime)) * 100  / total_delta_time,
            ((new_cpu.stime ) - (old_cpu.stime)) * 100 / total_delta_time,
            ((new_cpu.iowtime) - (old_cpu.iowtime)) * 100 / total_delta_time,
            ((new_cpu.irqtime + new_cpu.sirqtime)
                    - (old_cpu.irqtime + old_cpu.sirqtime)) * 100 / total_delta_time);
    printf("User %ld + Nice %ld + Sys %ld + Idle %ld + IOW %ld + IRQ %ld + SIRQ %ld = %ld\n",
            new_cpu.utime - old_cpu.utime,
            new_cpu.ntime - old_cpu.ntime,
            new_cpu.stime - old_cpu.stime,
            new_cpu.itime - old_cpu.itime,
            new_cpu.iowtime - old_cpu.iowtime,
            new_cpu.irqtime - old_cpu.irqtime,
            new_cpu.sirqtime - old_cpu.sirqtime,
            total_delta_time);
    printf("\n");
    if (!threads) {
        printf("%5s %2s %4s %1s %5s %9s %9s %3s %-8s %s\n", "PID", "PR", "CPU%", "S", "#THR", "VSS", "RSS", "PCY", "UID", "Name");        
    } else {
        printf("%5s %5s %2s %4s %1s %9s %9s %3s %-8s %-15s %s\n", "PID", "TID", "PR", "CPU%", "S", "VSS", "RSS", "PCY", "UID", "Thread", "Proc");
	}

    for (i = 0; i < num_new_procs; i++) {
        proc = new_procs[i];

        if (!proc || (max_procs && (i >= max_procs)))
            break;
        user  = getpwuid(proc->uid);
        group = getgrgid(proc->gid);
        if (user && user->pw_name) {
            user_str = user->pw_name;
        } else {
            snprintf(user_buf, 20, "%d", proc->uid);
            user_str = user_buf;
        }
        if (group && group->gr_name) {
            group_str = group->gr_name;
        } else {
            snprintf(group_buf, 20, "%d", proc->gid);
            group_str = group_buf;
        }
        if (!threads) {
            printf("%5d %2d %3ld%% %c %5d %8ldK %8ldK %3s %-8.8s %s\n", proc->pid, proc->prs, proc->delta_time * 100 / total_delta_time, proc->state, proc->num_threads,
                proc->vss / 1024, proc->rss * getpagesize() / 1024, proc->policy, user_str, proc->name[0] != 0 ? proc->name : proc->tname);
                
            if (is_show_files) {
				print_files(proc->pid);
			}    
                
        } else {
            printf("%5d %5d %2d %3ld%% %c %8ldK %8ldK %3s %-8.8s %-15s %s\n", proc->pid, proc->tid, proc->prs, proc->delta_time * 100 / total_delta_time, proc->state,
                proc->vss / 1024, proc->rss * getpagesize() / 1024, proc->policy, user_str, proc->tname, proc->name);
            
            if (is_show_stack) {    
				print_stack(proc->pid, proc->tid);    
			}
		}
    }
}

static struct proc_info *find_old_proc(pid_t pid, pid_t tid) {
    int i;

    for (i = 0; i < num_old_procs; i++)
        if (old_procs[i] && (old_procs[i]->pid == pid) && (old_procs[i]->tid == tid))
            return old_procs[i];

    return NULL;
}

static void free_old_procs(void) {
    int i;

    for (i = 0; i < num_old_procs; i++)
        if (old_procs[i])
            free_proc(old_procs[i]);

    free(old_procs);
}

static int proc_cpu_cmp(const void *a, const void *b) {
    struct proc_info *pa, *pb;

    pa = *((struct proc_info **)a); pb = *((struct proc_info **)b);

    if (!pa && !pb) return 0;
    if (!pa) return 1;
    if (!pb) return -1;

    return -numcmp(pa->delta_time, pb->delta_time);
}

static int proc_vss_cmp(const void *a, const void *b) {
    struct proc_info *pa, *pb;

    pa = *((struct proc_info **)a); pb = *((struct proc_info **)b);

    if (!pa && !pb) return 0;
    if (!pa) return 1;
    if (!pb) return -1;

    return -numcmp(pa->vss, pb->vss);
}

static int proc_rss_cmp(const void *a, const void *b) {
    struct proc_info *pa, *pb;

    pa = *((struct proc_info **)a); pb = *((struct proc_info **)b);

    if (!pa && !pb) return 0;
    if (!pa) return 1;
    if (!pb) return -1;

    return -numcmp(pa->rss, pb->rss);
}

static int proc_thr_cmp(const void *a, const void *b) {
    struct proc_info *pa, *pb;

    pa = *((struct proc_info **)a); pb = *((struct proc_info **)b);

    if (!pa && !pb) return 0;
    if (!pa) return 1;
    if (!pb) return -1;

    return -numcmp(pa->num_threads, pb->num_threads);
}

static int numcmp(long long a, long long b) {
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

static void usage(char *cmd) {
    fprintf(stderr, "Usage: %s [ -m max_procs ] [ -n iterations ] [ -d delay ] [ -s sort_column ] [ -t ] [ -h ]\n"
                    "    -m num  Maximum number of processes to display.\n"
                    "    -n num  Updates to show before exiting.\n"
                    "    -d num  Seconds to wait between updates.\n"                    
                    "    -s col  Column to sort by (cpu,vss,rss,thr).\n"
                    "    -p pid  Define pid (-p 1 2 3...).\n"
                    "    -t      Show threads instead of processes.\n"                    
                    "    -h      Display this help screen.\n"
                    "    --file  Show open files (only in processes).\n"
                    "    --stack Show thread stack (only in threads).\n",
        cmd);
}
