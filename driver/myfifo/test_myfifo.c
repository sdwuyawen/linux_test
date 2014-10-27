/**
	@filename :
		myfifo.c
**/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include <poll.h>

#include <sys/time.h>
#include <sys/resource.h>
#include <sys/syscall.h>

#define MEM_SIZE		4096
#define MEM_WR_SIZE		1024
#define MEM_RD_SIZE		1024

#define MYFIFO_CLEAR	0x01

//#define TEST_1_WR_1_RD
#define TEST_RD_POLL

#if 1
#define TEST_FLG_BLOCK		O_NONBLOCK
#else
#define TEST_FLG_BLOCK		0
#endif


/**
**/
static int g_fd;
static int g_is_exit;
static pthread_t pid;

static struct timeval g_tick_start;

/**
**/
static unsigned int test_get_ticks(void)
{
	struct timeval now;    
	unsigned int cur_tick;

	gettimeofday(&now, NULL);

	cur_tick = (now.tv_sec - g_tick_start.tv_sec)*1000 + (now.tv_usec - g_tick_start.tv_usec)/1000;

	return cur_tick;
}

/**
**/
static void *test_callback(void *pdata)
{
	char buf[MEM_SIZE];
	int ret;
	int cnt;
	unsigned int tick;

#if defined(TEST_1_WR_1_RD)
	cnt = 0;
	while (!g_is_exit)
	{
		usleep(500*1000);
		printf("[%d]ready read\n", cnt);	
		tick = test_get_ticks();
		ret = read(g_fd, buf, MEM_RD_SIZE);		
		if (ret != MEM_RD_SIZE)
		{
			g_is_exit = 1;
		}		
		printf("read = %d, %d\n", ret, (test_get_ticks() - tick));		
		cnt++;
	}
	printf("exit read loop\n");
#elif defined(TEST_RD_POLL)
	cnt = 0;
	while (!g_is_exit)
	{
		struct pollfd fds[1];
		
		printf("[%d]ready read\n", cnt);	
		
		tick = test_get_ticks();
		fds[0].fd = g_fd;
		fds[0].events = POLLIN;
		fds[0].revents = 0;
		ret = poll(fds, 1, 200);		//200ms
		if (ret < 0)
		{
			g_is_exit = 1;
			break;
		}
		
		if (ret == 0)
		{
			continue;
		}
		
		ret = read(g_fd, buf, MEM_RD_SIZE); 	
		if (ret != MEM_RD_SIZE)
		{
			g_is_exit = 1;
		}		
		printf("read = %d, %d\n", ret, (test_get_ticks() - tick));		
		cnt++;
	}
	printf("exit read loop\n");
#else
	printf("%s : ready read\n", __FUNCTION__);
	ret = read(g_fd, buf, MEM_RD_SIZE);
	printf("%s : read %d\n", __FUNCTION__, ret);
	sleep(1);
	printf("%s : ready write\n", __FUNCTION__);
	ret = write(g_fd, buf, MEM_RD_SIZE);
	printf("%s : write %d\n", __FUNCTION__, ret);	
#endif
	
	return NULL;
}

/**
**/
int main(void)
{
	int ret;
	int cnt;
	char buf[MEM_SIZE];
	unsigned int tick;
	
	gettimeofday(&g_tick_start, NULL);

	g_fd = open("/dev/myfifo", O_RDWR|TEST_FLG_BLOCK);
	if (g_fd < 0)
	{
		printf("open /dev/myfifo error!\n");
		return -1;
	}

	ret = ioctl(g_fd, MYFIFO_CLEAR, NULL);

	g_is_exit = 0;
	pthread_create(&pid, NULL, test_callback, NULL);

#if defined(TEST_1_WR_1_RD)
	cnt = 0;
	while (cnt < 10)
	{
		printf("[%d]ready write\n", cnt);
		tick = test_get_ticks();
		ret = write(g_fd, buf, MEM_RD_SIZE);
		if (ret < 0)
		{
			usleep(100*1000);
			continue;
		}
		printf("write = %d, %d\n", ret, (test_get_ticks()- tick));		
		usleep(100*1000);
		cnt++;
	}
	printf("exit write loop\n");
	//g_is_exit = 1;
	ret = write(g_fd, buf, 1);	
#elif defined(TEST_RD_POLL)	
	cnt = 0;
	while (cnt < 10)
	{
		printf("[%d]ready write\n", cnt);
		tick = test_get_ticks();
		ret = write(g_fd, buf, MEM_RD_SIZE);
		if (ret < 0)
		{
			usleep(100*1000);
			continue;
		}		
		printf("write = %d, %d\n", ret, (test_get_ticks()- tick));		
		usleep(100*1000);
		cnt++;
	}
	printf("exit write loop\n");
	//g_is_exit = 1;
	ret = write(g_fd, buf, 1);	
#else
	sleep(1);
	printf("%s : ready write\n", __FUNCTION__);
	ret = write(g_fd, buf, MEM_RD_SIZE);
	printf("%s : write %d\n", __FUNCTION__, ret);	
	usleep(500*1000);
	printf("%s : ready read\n", __FUNCTION__);
	ret = read(g_fd, buf, MEM_RD_SIZE);
	printf("%s : read %d\n", __FUNCTION__, ret);
#endif
	pthread_join(pid, NULL);
	close(g_fd);

	return 0;	
}

