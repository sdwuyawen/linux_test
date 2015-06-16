/**
 * @filename :
 * 		test_timerfd.c
 * 
 * @note :
 * 	1. it_value和it_interval的时间不能设置为0, 0表示停止当前timer
 **/
#define _USE_EPOLL
 
#include <unistd.h>
#include <sys/timerfd.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#ifdef _USE_EPOLL
#include <sys/epoll.h>
#else
#include <poll.h>
#endif
#include <stdio.h>

#ifdef _USE_EPOLL
static struct epoll_event events[32];
#else
static struct pollfd pollfds[2];
#endif

int main(int argc, char **argv){
  int fd = -1;
  int epfd = -1;
  int ret;  
  
  struct itimerspec its = {
    .it_value.tv_sec = 1,
    .it_value.tv_nsec = 0,
    .it_interval.tv_sec = 1,
    .it_interval.tv_nsec = 0,
  };
  
#ifdef _USE_EPOLL  
  if ((epfd = epoll_create(32000)) == -1) {
	  printf("epooll error");
	  return -1;
  }
#endif
  
  fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  if(fd < 0){
    perror("timerfd_create");
    goto _error1;
  }
  
  if(timerfd_settime(fd, 0, &its, NULL) < 0) {
    perror("timerfd_settime");
    goto _error1;
  }
  
#ifdef _USE_EPOLL    
  struct epoll_event epev;
  memset(&epev, 0, sizeof(epev));
  epev.data.fd = fd;
  epev.events = EPOLLIN;
  if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &epev) < 0) {
    printf("add timerfd error!\n");
    goto _error1;
  }  
#else  
  pollfds[0].fd = fd;
  pollfds[0].events = POLLIN;	
#endif
  
  uint64_t exp;
  ssize_t s;
    
  while(true){	 
#ifdef _USE_EPOLL  	  
	ret = epoll_wait(epfd, events, 1, -1);
#else	
	ret = poll(pollfds, 1, -1);
#endif	
	if (ret < 0) {
		printf("poll error\n");
		break;
	} else if (ret == 0) {
		continue;
	}

    s = read(fd, &exp, sizeof(uint64_t));
    if(s == -1 && errno == ECANCELED){
      printf("time changed.\n");
    }else{
      printf("s=%zd, exp=%llu\n", s, exp);
    }
  }

_error1:  
  if (epfd != -1) {
	close(epfd);
  }
  
  close(fd);
  
  return 0;
}
