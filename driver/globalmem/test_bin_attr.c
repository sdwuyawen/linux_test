/**
	@filename :
		test_bin_attr.c
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

/**
**/
#define DEMO_BIN_ATTR_NAME		"/sys/devices/platform/demoDev.0/eeprom"

/**
**/
int main(int argc, char **argv)
{
	int fd = 0;
	int filesize;
	char buf[1024];
	int ret;
	
	fd = open(DEMO_BIN_ATTR_NAME, O_RDWR);
	if (fd < 0)	
	{
		printf("can not open %s\n", DEMO_BIN_ATTR_NAME);
		return -1;
	}
	
	filesize = lseek(fd, 0, 2);	
	printf("filesize = %d\n", filesize);
		
	sprintf(buf, DEMO_BIN_ATTR_NAME);	
	lseek(fd, 0, 0);	
	write(fd, buf, 1024);
	
	memset(buf, 0, 1024);
	lseek(fd, 0, 0);	
	ret = read(fd, buf, 1024);
	printf("ret = %d, %s\n", ret, buf);
		
	close(fd);
	
	return 0;
}