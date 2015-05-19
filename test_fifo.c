/**
 * 	@filename :
 * 		test_fifo.c
 * 
 * 	@note :
 * 
 **/
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#if 0
#define FIFO_R_NAME	"/var/fifo_read"
#define FIFO_W_NAME	"/var/fifo_write"
#else
#define FIFO_R_NAME	"/tmp/fifo_read"
#define FIFO_W_NAME	"/tmp/fifo_write"
#endif

void main(void)
{
	int s_fd_rd, s_fd_wr;
	int c_fd_rd, c_fd_wr;
	char *hello_str = "hello to server";
	char *ack_str = "ack to client";
	char buffer[256];
	int ret;

	unlink(FIFO_R_NAME);
	unlink(FIFO_W_NAME);	
	mkfifo(FIFO_R_NAME, O_CREAT|O_EXCL|0777);
	mkfifo(FIFO_W_NAME, O_CREAT|O_EXCL|0777);
	
	//server
	printf("s_fd_rd\n");
	s_fd_rd = open(FIFO_R_NAME, O_RDONLY | O_NONBLOCK);
	if (s_fd_rd < 0) {
		perror("s_fd_rd");
	}
	printf("s_fd_wr\n");
	/*
	如果是O_WRONLY, 就会block直到c_fd_rd = open(FIFO_W_NAME, O_RDONLY | O_NONBLOCK)调用成功;
	如果是O_WRONLY | O_NONBLOCK, 就会open失败.
	解决方法: 
		1.改为O_RDWR.
		2.在另一个线程调用c_fd_rd = open(FIFO_W_NAME, O_RDONLY | O_NONBLOCK)之后，阻塞才能解除.
	*/
	s_fd_wr = open(FIFO_W_NAME, O_RDWR);		
	if (s_fd_rd < 0) {
		perror("s_fd_wr");
	}	
	
	//delay
	printf("delay...\n");
	sleep(1);
	
	//client
	printf("c_fd_rd\n");
	c_fd_rd = open(FIFO_W_NAME, O_RDONLY | O_NONBLOCK);
	if (s_fd_rd < 0) {
		perror("c_fd_rd");
	}	
	printf("c_fd_wr\n");
	c_fd_wr = open(FIFO_R_NAME, O_WRONLY);	
	if (s_fd_rd < 0) {
		perror("c_fd_wr");
	}	

	printf("====== hello to server\n");
	ret = write(c_fd_wr, hello_str, strlen(hello_str));
	if (ret < 0) {
		perror("c write hello");
	}
	printf("c write : ret = %d\n", ret);
	usleep(500);
	memset(buffer, 0, sizeof(buffer));
	ret = read(s_fd_rd, buffer, sizeof(buffer));
	if (ret < 0) {
		perror("s read hello");
	}	
	printf("s read : ret = %d,  %s\n", ret, buffer);
	
	printf("====== ack to client\n");
	ret = write(s_fd_wr, ack_str, strlen(ack_str));
	if (ret < 0) {
		perror("s write ack");
	}
	printf("s write : ret = %d\n", ret);
	usleep(500);
	memset(buffer, 0, sizeof(buffer));
	ret = read(c_fd_rd, buffer, sizeof(buffer));
	if (ret < 0) {
		perror("c read ack");
	}	
	printf("c read : ret = %d,  %s\n", ret, buffer);	
	
	
	close(s_fd_rd);
	close(s_fd_wr);
	close(c_fd_rd);
	close(c_fd_wr);	
	
}
