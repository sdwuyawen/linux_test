/**
 * 	@filename :
 * 		test_ns_user.c
 * 
 * 	@note :
 * 		通过CLONE_NEWUSER来User namespace隔离。
 * 
1. 尚未与外部namespace用户映射 
paul@wubaoqiang:~/work/test/linux_test$ sudo ./bin/test_ns_user 
程序开始: 
在子进程中!
eUID = 65534;  eGID = 65534;  capabilities: = cap_chown,cap_dac_override,cap_dac_read_search,cap_fowner,cap_fsetid,cap_kill,cap_setgid,cap_setuid,cap_setpcap,cap_linux_immutable,cap_net_bind_service,cap_net_broadcast,cap_net_admin,cap_net_raw,cap_ipc_lock,cap_ipc_owner,cap_sys_module,cap_sys_rawio,cap_sys_chroot,cap_sys_ptrace,cap_sys_pacct,cap_sys_admin,cap_sys_boot,cap_sys_nice,cap_sys_resource,cap_sys_time,cap_sys_tty_config,cap_mknod,cap_lease,cap_audit_write,cap_audit_control,cap_setfcap,cap_mac_override,cap_mac_admin,cap_syslog,cap_wake_alarm,cap_block_suspend+ep
nobody@wubaoqiang:~/work/test/linux_test$ exit
exit
已退出
paul@wubaoqiang:~/work/test/linux_test$ 

2. 与外部namespace用户映射 

 * 
 **/
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

#include <sys/capability.h> 
 
#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];
char* const child_args[] = {
  "/bin/bash",
  NULL
};

void set_uid_map(pid_t pid, int inside_id, int outside_id, int length) {
    char path[256];
    printf("%s\n", __func__);
    sprintf(path, "/proc/%d/uid_map", getpid());
    FILE* uid_map = fopen(path, "w");
    fprintf(uid_map, "%d %d %d", inside_id, outside_id, length);
    fclose(uid_map);
}

void set_gid_map(pid_t pid, int inside_id, int outside_id, int length) {
    char path[256];
    printf("%s\n", __func__);
    sprintf(path, "/proc/%d/gid_map", getpid());
    FILE* gid_map = fopen(path, "w");
    fprintf(gid_map, "%d %d %d", inside_id, outside_id, length);
    fclose(gid_map);
}

int child_main(void* args) {
  printf("在子进程中!\n");
  cap_t caps;
  set_uid_map(getpid(), 0, 1000, 1);
  set_gid_map(getpid(), 0, 1000, 1);  
  printf("eUID = %ld;  eGID = %ld;  ",
				(long) geteuid(), (long) getegid());
  caps = cap_get_proc();
  printf("capabilities: %s\n", cap_to_text(caps, NULL));
  execv(child_args[0], child_args);
  return 1;
}

int main() {
  printf("程序开始: \n");
  int child_pid = clone(child_main, child_stack + STACK_SIZE, CLONE_NEWUSER | SIGCHLD, NULL);
  if (child_pid < 0) {
	  goto _exit;
  }
  waitpid(child_pid, NULL, 0);
_exit:  
  printf("已退出\n");
  return 0;
} 
