/*
 * test_inotify.c
 * 
 * Copyright 2012 paul <paul@paul-desktop>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */
 /*
  *		sudo ./bin/test_inotify
  *  
  */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>
#include <errno.h>

#include <sys/inotify.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
//#include <sys/limits.h>

#include <linux/input.h>

/**
 **/
#define EPOLL_SIZE_HINT 		8
#define EPOLL_MAX_EVENTS		16	
#define DEVICE_PATH				"/dev/input"

#define EPOLL_ID_INOTIFY		0x80000001
 
/**
 **/
static int g_epoll_fd;
static int g_inotify_fd;

/*
 * 
 * name: readNotifyEvent
 * @param
 * @return
 */
static int checkDevice(const char *devicePath) {
    char buffer[80];

    fprintf(stderr, "checkDevice device: %s\n", devicePath);

    int fd = open(devicePath, O_RDWR);
    if(fd < 0) {
        fprintf(stderr, "could not open %s, %s\n", devicePath, strerror(errno));
        return -1;
    }

    // Get device name.
    if(ioctl(fd, EVIOCGNAME(sizeof(buffer) - 1), &buffer) < 1) {
        fprintf(stderr, "could not get device name for %s, %s\n", devicePath, strerror(errno));
    } 
    buffer[sizeof(buffer) - 1] = '\0';
	printf("  name:       \"%s\"\n", buffer);
    	
    // Get device driver version.
    int driverVersion;
    if(ioctl(fd, EVIOCGVERSION, &driverVersion)) {
        fprintf(stderr, "could not get driver version for %s, %s\n", devicePath, strerror(errno));
        close(fd);
        return -1;
    }

    printf("  driver:     v%d.%d.%d\n",
        driverVersion >> 16, (driverVersion >> 8) & 0xff, driverVersion & 0xff);

    // Get device identifier.
    struct input_id inputId;
    if(ioctl(fd, EVIOCGID, &inputId)) {
        fprintf(stderr, "could not get device input id for %s, %s\n", devicePath, strerror(errno));
        close(fd);
        return -1;
    }

    // Get device physical location.
    if(ioctl(fd, EVIOCGPHYS(sizeof(buffer) - 1), &buffer) < 1) {
        fprintf(stderr, "could not get location for %s, %s\n", devicePath, strerror(errno));
    } 
    buffer[sizeof(buffer) - 1] = '\0';
	printf("  location:   \"%s\"\n", buffer);    
	
    // Get device unique id.
    if(ioctl(fd, EVIOCGUNIQ(sizeof(buffer) - 1), &buffer) < 1) {
        fprintf(stderr, "could not get idstring for %s, %s\n", devicePath, strerror(errno));
    } 
    buffer[sizeof(buffer) - 1] = '\0';
	printf("  unique id:  \"%s\"\n", buffer);
    
    printf("  bus:        %04x\n"
         "  vendor      %04x\n"
         "  product     %04x\n"
         "  version     %04x\n",
        inputId.bustype, inputId.vendor, inputId.product, inputId.version);
                
    close(fd);
    
    return 0;
}

/*
 * 
 * name: readNotifyEvent
 * @param
 * @return
 */
static int readNotifyEvent(void) 
{
    int res;
    char devname[PATH_MAX];
    char *filename;
    char event_buf[512];
    int event_size;
    int event_pos = 0;
    struct inotify_event *event;

    res = read(g_inotify_fd, event_buf, sizeof(event_buf));
    if(res < (int)sizeof(*event)) {
        if(errno == EINTR)
            return 0;
        printf("could not get event, %s\n", strerror(errno));
        return -1;
    }
    //printf("got %d bytes of event information\n", res);

    strcpy(devname, DEVICE_PATH);
    filename = devname + strlen(devname);
    *filename++ = '/';

    while(res >= (int)sizeof(*event)) {
        event = (struct inotify_event *)(event_buf + event_pos);
        //printf("%d: %08x \"%s\"\n", event->wd, event->mask, event->len ? event->name : "");
        if(event->len) {
            strcpy(filename, event->name);
            if(event->mask & IN_CREATE) {
				fprintf(stderr, "Add device '%s' due to inotify event\n", devname);
				checkDevice(devname);
            } else {
                fprintf(stderr, "Removing device '%s' due to inotify event\n", devname);
            }
        }
        event_size = sizeof(*event) + event->len;
        res -= event_size;
        event_pos += event_size;
    }
    
    return 0;
}

/*
 * 
 * name: main
 * 
 * @param
 * 	@@argc
 *  @@argv
 * 
 * @return
 * 	0
 */
int main(int argc, char **argv)
{
	int result;
	int i;
    struct epoll_event event_item;    	
	struct epoll_event pending_event_items[EPOLL_MAX_EVENTS];
	
	printf("argc = %d, argv[0] = %s\n", argc, argv[0]);

	g_epoll_fd = epoll_create(EPOLL_SIZE_HINT);

    g_inotify_fd = inotify_init();
    result = inotify_add_watch(g_inotify_fd, DEVICE_PATH, IN_DELETE | IN_CREATE);
    printf("inotify_add_watch result = %d\n", result);
    
    memset(&event_item, 0, sizeof(event_item));
    event_item.events = EPOLLIN;
    event_item.data.u32 = EPOLL_ID_INOTIFY;
    result = epoll_ctl(g_epoll_fd, EPOLL_CTL_ADD, g_inotify_fd, &event_item);	
	while (1)
	{
		result = epoll_wait(g_epoll_fd, pending_event_items, EPOLL_MAX_EVENTS, -1);
		if (result <= 0)
		{
			sleep(1);
			continue;
		}
		
		for (i=0; i<result; i++)
		{
			fprintf(stderr, "============= [0x%x] ============\n", pending_event_items[i].data.u32);
			readNotifyEvent();	
			fprintf(stderr, "=================================\n");	
		}
		
		
	}
	
	close(g_inotify_fd);
	close(g_epoll_fd);
	
	return 0;
} 

 
