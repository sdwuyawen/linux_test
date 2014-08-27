/**
 * 	@filename :
 * 		test_futex.c
 * 
 *  @note :
 * 		futex(&pinfo->serial, FUTEX_WAIT, serial, NULL, NULL, 0);
 * 		如果	pinfo->serial != serial, 直接返回
 * 		如果	pinfo->serial == serial, 等待wake
 * 
 **/
#include <stdio.h>
#include <linux/futex.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/syscall.h>

#define futex(addr1, op, val, rel, addr2, val3)    \
    syscall(SYS_futex, addr1, op, val, rel, addr2, val3)
        
/**
 **/
typedef struct {
	 void *data;
	 unsigned int size;
	 int fd;
}workspace_t;

typedef struct {
	int serial;
}work_info_t;

#define PA_COUNT_MAX  372
#define PA_INFO_START 1536
#define PA_SIZE       49152

#define PA_FILENAME	  "/tmp/__properties__"	

static int test_futex_child(void)    
{
	void *data;
	int fd;
	char *env;
	int size;
	work_info_t *pinfo;
	
	printf("test_futex_child\n"); 	
	env = (char *)getenv((char *)"TEST_PROP");
	if (!env) {
		printf("no env\n");
		return -1;
	}
	
	fd = atoi(env);
	env = strchr(env, ',');
	if (!env) {
		printf("env error!\n");
		return -1;
	}
	size = atoi(env + 1);	
	
	data = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);	 
	if(data == MAP_FAILED) {
		printf("mmap error!\n");
		close(fd);
		return -1;
	}	 
	
	pinfo = (work_info_t *)data;
	pinfo->serial = 1;
	printf("child wake\n");
	futex(&pinfo->serial, FUTEX_WAKE, INT32_MAX, NULL, NULL, 0);
	
	munmap(data, size);	 
	close(fd);
	
	return 0;
}


/**
 **/
static int init_workspace(workspace_t *w, size_t size)
{
    void *data;
    int fd;

        /* dev is a tmpfs that we can use to carve a shared workspace
         * out of, so let's do that...
         */
    fd = open(PA_FILENAME, O_RDWR | O_CREAT, 0600);
    if (fd < 0)
        return -1;

    if (ftruncate(fd, size) < 0)
        goto out;

    data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(data == MAP_FAILED)
        goto out;

    close(fd);

    fd = open(PA_FILENAME, O_RDWR);	
    if (fd < 0)
        return -1;

    unlink(PA_FILENAME);

    w->data = data;
    w->size = size;
    w->fd = fd;
    return 0;

out:
    close(fd);
    return -1;
}


/**
	sudo ./bin/test_devfs
 **/
int main(int argc, char *argv[]) {
	int ret;
	workspace_t w;
	pid_t pid;
	char prop_buf[256];
	work_info_t *pinfo;
	int serial;
	 
	ret = init_workspace(&w, PA_SIZE);
	if (ret < 0) {
		  printf("init_workspace error!\n");
		  return -1;
	}

	fcntl(w.fd, F_SETFD, FD_CLOEXEC);

	pid = fork();
	if (pid < 0) {
		  /* error */
		  _exit(127);
	} else  if (pid == 0) {
		/* child */
		printf("child\n");
		sprintf(prop_buf, "%d,%d", dup(w.fd), w.size); /* dup(w.fd) : 复制w.fd, 在子进程可以访问这个文件句柄. */
		setenv("TEST_PROP", prop_buf, 1);		  
		sleep(1);
		test_futex_child();
		_exit(127);
	} else {
		/* parent */
		printf("parent\n");
		pinfo = (work_info_t *)w.data;
		serial = 0;
		pinfo->serial = 0;
		printf("futex wait enter %d\n", pinfo->serial);
		futex(&pinfo->serial, FUTEX_WAIT, serial, NULL, NULL, 0);
		printf("futex wait exit %d\n", pinfo->serial);
		wait(NULL);
		printf("exit %d\n", pinfo->serial);
	}
	 
	printf("%s : wait any key for exit...\n", argv[0]);
	getchar();

	munmap(w.data, w.size);	 
	close(w.fd);
	 
	 return 0;
}
