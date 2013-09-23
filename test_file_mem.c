/**
 *	@filename :
 * 		test_file_mem.c
 * 
 * 	@note :
 * 		如果是map file, 父子进程可以读写相同的内存 (不确定是否安全)。
 * 		如果是malloc, 父子进程读写相同的内存时, 会出现write copy, 所以读写出来的内容不一样。		 
 * 
 **/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>

/**
 **/
typedef struct {
	 void *data;
	 unsigned int size;
	 int fd;
	 int type;
} workspace_t;

#define PA_SIZE       4096

#define TEST_VM_TYPE_FILE	0		
#define TEST_VM_TYPE_MEM	1
#define TEST_VM_TYPE_MAX	2

/**
 **/
static int init_workspace(workspace_t *w, size_t size, int type)
{
    void *data;
    int fd = 0;

	if (type == TEST_VM_TYPE_FILE) {
			/* dev is a tmpfs that we can use to carve a shared workspace
			 * out of, so let's do that...
			 */
		fd = open("/dev/__properties__", O_RDWR | O_CREAT, 0600);
		if (fd < 0) {
			return -1;
		}

		if (ftruncate(fd, size) < 0) {
			close(fd);
			return -1;
		}

		data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if(data == MAP_FAILED) {
			close(fd);
			return -1;
		}

		close(fd);

		fd = open("/dev/__properties__", O_RDONLY);
		if (fd < 0) {
			return -1;
		}
		fcntl(fd, F_SETFD, FD_CLOEXEC);
		unlink("/dev/__properties__");

	} else if (type == TEST_VM_TYPE_MEM) {		
		data = malloc(size);
		if (data == NULL) {
			return -1;
		}
		
	} else {
		return -1;
	}
	
    w->data = data;
    w->size = size;
    w->fd = fd;
    w->type = type;
	
	return 0;

}

static char *g_vm_type_name[TEST_VM_TYPE_MAX] = {
	"file",
	"mem"
};

int main(int argc, char *argv[]) {
	int ret;
	workspace_t w;
	workspace_t w1;
	unsigned int *pdata;
	int i;
	pid_t pid;
	
	ret = init_workspace(&w, PA_SIZE, TEST_VM_TYPE_FILE);
	if (ret < 0) {
		printf("init_workspace w error!\n");
		return -1;
	}	
	memset(w.data, 0, w.size);
	
	ret = init_workspace(&w1, PA_SIZE, TEST_VM_TYPE_MEM);
	if (ret < 0) {	
		close(w.fd);	
		printf("init_workspace w1 error!\n");
		return -1;
	}	
	memset(w1.data, 0, w1.size);

	sprintf(w.data, "test");
	sprintf(w1.data, "test");
		
	pid = fork();
	if (pid == 0) {
		printf("\n=========== Child R ==========\n");
		printf("[%-4s]: %s\n", g_vm_type_name[w.type], (char *)w.data);
		printf("[%-4s]: %s\n", g_vm_type_name[w1.type], (char *)w1.data);
		sprintf(w.data, "hello world");
		sprintf(w1.data, "hello world");
		printf("\n=========== Child W ==========\n");
		printf("[%-4s]: %s\n", g_vm_type_name[w.type], (char *)w.data);
		printf("[%-4s]: %s\n", g_vm_type_name[w1.type], (char *)w1.data);
	} else {
		wait(NULL);
		printf("\n========== Parent R ==========\n");
		printf("[%-4s]: %s\n", g_vm_type_name[w.type], (char *)w.data);
		printf("[%-4s]: %s\n", g_vm_type_name[w1.type], (char *)w1.data);
		printf("\n");
	}
	
	if (w.fd) {
		close(w.fd);
	}
	
	if (w1.data > 0) {
		free(w1.data);
	}
	 	
	return 0;
}
