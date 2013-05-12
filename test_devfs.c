/**
   @filename :
   		test_devfs.c
**/
#include <stdio.h>
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

/**
 **/
typedef struct {
	 void *data;
	 unsigned int size;
	 int fd;
} workspace_t;


#define PA_COUNT_MAX  372
#define PA_INFO_START 1536
#define PA_SIZE       49152

/**
 **/
static int init_workspace(workspace_t *w, size_t size)
{
    void *data;
    int fd;

        /* dev is a tmpfs that we can use to carve a shared workspace
         * out of, so let's do that...
         */
    fd = open("/dev/__properties__", O_RDWR | O_CREAT, 0600);
    if (fd < 0)
        return -1;

    if (ftruncate(fd, size) < 0)
        goto out;

    data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(data == MAP_FAILED)
        goto out;

    close(fd);

    fd = open("/dev/__properties__", O_RDONLY);
    if (fd < 0)
        return -1;

    unlink("/dev/__properties__");

    w->data = data;
    w->size = size;
    w->fd = fd;
    return 0;

out:
    close(fd);
    return -1;
}


/**
 **/
int main(int argc, char *argv[]) {
	 int ret;
	 workspace_t w;
	 pid_t pid;
	 char prop_buf[256];
	 
	 ret = init_workspace(&w, PA_SIZE);
	 if (ret < 0) {
		  printf("init_workspace error!\n");
		  return -1;
	 }

	 fcntl(w.fd, F_SETFD, FD_CLOEXEC);

	 sprintf(w.data, "hello world\n");
	 printf("rd : %s\n", (char *)w.data);

	 pid = fork();
	 if (pid < 0) {
		  /* error */
	 } else  if (pid == 0) {
		  /* child */
		  printf("child\n");
		  sprintf(prop_buf, "%d,%d", dup(w.fd), w.size); /* dup(w.fd) is used in child process. */
		  setenv("TEST_PROP", prop_buf, 1);
		  printf("exec test_getprog\n");
		  ret = execl("/home/paul2/work/test/linux_test/bin/test_getprop", "test_getprop", NULL);
		  printf("ret = %d, %s\n", ret, strerror(errno));
		  _exit(127);
	 } else {
		  /* parent */
		  printf("parent\n");
		  wait(NULL);
		  printf("exit\n");
	 }
	 

	 printf("get any key for exit...\n");
	 getchar();
	 
	 close(w.fd);
	 
	 return 0;
}