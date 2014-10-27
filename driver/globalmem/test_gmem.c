/**
	@filename :
		test_gmem.c
		
	@descriptions:
		test globalmem	
**/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/**
**/
#define GLOBALMEM_SIZE 0x1000 /*4KB*/

//#define GLOBALMEM_TEST_TWO

#define MEM_CLEAR 0x1  /*clear globale memory*/

/**
**/
static char g_mem_buf[GLOBALMEM_SIZE];

/**
**/
int main(void)
{
	int fd;
	int fd1;
	int ret;
	unsigned int data;
	
	fd = open("/dev/globalmem", O_RDWR);
	if (fd < 0)
	{
		printf("open /dev/globalmem error\n");
		return -1;
	}

#ifdef GLOBALMEM_TEST_TWO
	fd1 = open("/dev/globalmem1", O_RDWR);
#else
	fd1 = open("/dev/globalmem", O_RDWR);
#endif
	if (fd1 < 0)
	{
		close(fd);
		printf("open /dev/globalmem1 error\n");
		return -1;
	}
	
	sprintf(g_mem_buf, "globalmem size = %d", GLOBALMEM_SIZE);
	
	printf("[%x][buf0 ] %s\n", fd, g_mem_buf);
	ret = write(fd, g_mem_buf, GLOBALMEM_SIZE);
	printf("[%x]write = %d\n", fd, ret);

	ret = write(fd1, g_mem_buf, GLOBALMEM_SIZE);
	printf("[%x]write = %d\n", fd1, ret);
	
	memset(g_mem_buf, 0, GLOBALMEM_SIZE);
	lseek(fd, 0, 0);
	ret = read(fd, g_mem_buf, GLOBALMEM_SIZE);
	printf("[%x]read0 = %d\n", fd, ret);
	if (ret)
	{
		printf("[%x][data0] %s\n", fd, g_mem_buf);
	}

	ret = ioctl(fd, MEM_CLEAR, 0);
	printf("[%x]ioctl ret = %d\n", fd, ret);
	memset(g_mem_buf, 0, GLOBALMEM_SIZE);
	lseek(fd, 0, 0);
	ret = read(fd, g_mem_buf, GLOBALMEM_SIZE);
	printf("[%x]read1 = %d\n", fd, ret);
	if (ret)
	{
		printf("[%x][data1] %s\n", fd, g_mem_buf);
	}

	lseek(fd1, 0, 0);
	ret = read(fd1, g_mem_buf, GLOBALMEM_SIZE);
	printf("[%x]read1 = %d\n", fd1, ret);
	if (ret)
	{
		printf("[%x][data1] %s\n", fd1, g_mem_buf);
	}
	
	close(fd1);		
	close(fd);
	
	return 0;
}


