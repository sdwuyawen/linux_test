/**
 * 	@filename :
 * 		test_time.c
 * 
 * 	@note :
 * 		1376050008953
 * 			单位为ms
 * 
 * 		./test_time
 * 			获取当前日期时间
 * 
 * 		./test_time 1376050008953
 * 			显示1376050008953所代表的日期时间
 * 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
	time_t tt;
	char time_str[256]={0};
	struct tm* tp;
	struct timezone tz;
	struct timeval tv;
	int ret;
	
	if (argc > 1) {
		int len;
		char buf[256];
		
		strcpy(buf, argv[1]);
		len = strlen(buf);
		
		tv.tv_usec = atol(&buf[len-3])*1000;
		buf[len-3] = 0;
		tv.tv_sec = atol(&buf[0]);		
	} else {
		//current time
		ret = gettimeofday(&tv, &tz);
	}	
	
	printf("gettime : [sec]%d, [usec]%d\n", (int)tv.tv_sec, (int)tv.tv_usec);
	
	tt = tv.tv_sec;
	tp = localtime(&tt);
	strftime(time_str, 100, "%Y-%m-%d %H:%M:%S", tp); 	
		
	printf("localtime : %s [usec]%d\n", time_str, (int)tv.tv_usec);	
	
	return 0;
}
