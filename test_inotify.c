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

#define EVENTS_RD_NUM			16

#define DEVICE_EVENT_NAME		"event"
#define DEVICE_EVENT_NAME_LEN	5

#define EPOLL_ID_INOTIFY		0x80000001

typedef struct _input_device_t {	
	struct _input_device_t *next;
	char name[128];
	int hash_code;
	int fd;
}input_device_t;

 
/**
 **/
static int g_epoll_fd;
static int g_inotify_fd;
static int g_device_cnt = 0;

static input_device_t *g_device_list = NULL;

/*
 * 
 * name: get_hash_code
 * @param
 * @return
 */
static int get_hash_code(const char *str)
{//微软System.String 字符串哈希算法
	char* chPtr = (char *)str;
	int num = 0x15051505;
	int num2 = num;
	int *numPtr = (int*)chPtr;
	int i, len;
	
	i = strlen(str);
	for (; i > 0; i -= 4) {
		num = (((num << 5) + num) + (num >> 0x1b)) ^ numPtr[0];
		if (i <= 2) {
			break;
		}
		num2 = (((num2 << 5) + num2) + (num2 >> 0x1b)) ^ numPtr[1];
		numPtr += 2;
	}
	
	return (num + (num2 * 0x5d588b65));
}
/*
 * 
 * name: open_device
 * @param
 * @return
 */
static int open_device(const char *dev_path) {
    char buffer[80];
    int result;
	input_device_t *pdev;
	input_device_t *pnext;
	struct epoll_event event_item;    		
	
    fprintf(stderr, "open_device device: %s\n", dev_path);

    int fd = open(dev_path, O_RDWR);
    if(fd < 0) {
        fprintf(stderr, "could not open %s, %s\n", dev_path, strerror(errno));
        return -1;
    }

    // Get device name.
    if(ioctl(fd, EVIOCGNAME(sizeof(buffer) - 1), &buffer) < 1) {
        fprintf(stderr, "could not get device name for %s, %s\n", dev_path, strerror(errno));
    } 
    buffer[sizeof(buffer) - 1] = '\0';
	printf("  name:       \"%s\"\n", buffer);
    	
    // Get device driver version.
    int driverVersion;
    if(ioctl(fd, EVIOCGVERSION, &driverVersion)) {
        fprintf(stderr, "could not get driver version for %s, %s\n", dev_path, strerror(errno));
        close(fd);
        return -1;
    }

    printf("  driver:     v%d.%d.%d\n",
        driverVersion >> 16, (driverVersion >> 8) & 0xff, driverVersion & 0xff);

    // Get device identifier.
    struct input_id inputId;
    if(ioctl(fd, EVIOCGID, &inputId)) {
        fprintf(stderr, "could not get device input id for %s, %s\n", dev_path, strerror(errno));
        close(fd);
        return -1;
    }

    // Get device physical location.
    if(ioctl(fd, EVIOCGPHYS(sizeof(buffer) - 1), &buffer) < 1) {
        fprintf(stderr, "could not get location for %s, %s\n", dev_path, strerror(errno));
    } 
    buffer[sizeof(buffer) - 1] = '\0';
	printf("  location:   \"%s\"\n", buffer);    
	
    // Get device unique id.
    if(ioctl(fd, EVIOCGUNIQ(sizeof(buffer) - 1), &buffer) < 1) {
        fprintf(stderr, "could not get idstring for %s, %s\n", dev_path, strerror(errno));
    } 
    buffer[sizeof(buffer) - 1] = '\0';
	printf("  unique id:  \"%s\"\n", buffer);
    
    printf("  bustype:        %04x\n"
         "  vendor      %04x\n"
         "  product     %04x\n"
         "  version     %04x\n",
        inputId.bustype, inputId.vendor, inputId.product, inputId.version);

	pdev = malloc(sizeof(input_device_t));
	if (pdev == NULL) {
		close(fd);
		return -1;
	}
	
	strcpy(pdev->name, dev_path);
	pdev->hash_code = get_hash_code(pdev->name);
	pdev->fd = fd;
	
	pnext = g_device_list;
	g_device_list = pdev;
	g_device_list->next = pnext;

    memset(&event_item, 0, sizeof(event_item));
    event_item.events = EPOLLIN;
    event_item.data.u32 = (uint32_t)pdev;
    result = epoll_ctl(g_epoll_fd, EPOLL_CTL_ADD, fd, &event_item);	
                        
    return 0;
}

/*
 * 
 * name: close_device
 * @param
 * @return
 */
static int close_device(const char *dev_path) {
	int hash_code;
	struct epoll_event event_item;    	
	int result;
	input_device_t *pdev;
	input_device_t *prev;
	
	fprintf(stderr, "close_device device: %s\n", dev_path);
	
	hash_code = get_hash_code(dev_path);
	if (g_device_list == NULL) {
		return -1;
	}
	
	pdev = g_device_list;
	prev = g_device_list;
	do {
		if (pdev->hash_code == hash_code) {
			break;
		}
		prev = pdev;
	} while ((pdev = pdev->next) != NULL);
	
	if (pdev != NULL) {
		if (pdev == prev) {
			g_device_list = pdev->next;
		} else {
			prev->next = pdev->next;
		}
		
		memset(&event_item, 0, sizeof(event_item));
		event_item.events = EPOLLIN;
		event_item.data.u32 = pdev;
		result = epoll_ctl(g_epoll_fd, EPOLL_CTL_DEL, pdev->fd, &event_item);	
    	
		close(pdev->fd);
		
		free(pdev);
	}
	
	return 0;
}

/*
 * 
 * name: read_notify_event
 * @param
 * @return
 */
static int read_notify_event(void) 
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
    printf("got %d bytes of event information\n", res);

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
				open_device(devname);
            } else {
                fprintf(stderr, "Removing device '%s' due to inotify event\n", devname);
                close_device(devname);
            }
        }
        event_size = sizeof(*event) + event->len;
        res -= event_size;
        event_pos += event_size;
    }
    
    return 0;
}

/**
**/
static int scan_device_dir(const char *dir_name)
{
    char devname[PATH_MAX];
    char *filename;
    DIR *dir;
    struct dirent *de;
	int id;
	
    dir = opendir(dir_name);
    if(dir == NULL)
        return -1;
	
    strcpy(devname, dir_name);
    filename = devname + strlen(devname);
    *filename++ = '/';
    while((de = readdir(dir))) {
        if(de->d_name[0] == '.' &&
           (de->d_name[1] == '\0' ||
            (de->d_name[1] == '.' && de->d_name[2] == '\0')))
            continue;
		
		//if (!strncmp(de->d_name, DEVICE_EVENT_NAME, DEVICE_EVENT_NAME_LEN))
		{								
	        strcpy(filename, de->d_name);
    	    open_device(devname);
		}
    }
	
    closedir(dir);
	
	return 0;
}

static int read_input_event(int fd) 
{
	int i;
	int read_size;
	int event_cnt;
	struct input_event event_buf[EVENTS_RD_NUM];
	
	read_size = read(fd, event_buf, sizeof(struct input_event) * EVENTS_RD_NUM);
	if (read_size < 0) {
		return -1;		
	}
	
	event_cnt = read_size/sizeof(struct input_event);
	for (i=0; i<event_cnt; i++) {
		//printf("type = %d, code = %d, value = %d\n", event_buf[i].type, event_buf[i].code, event_buf[i].value);
		switch(event_buf[i].type) {
			case EV_KEY:
				printf("key = %d, value = %d\n", event_buf[i].code, event_buf[i].value);
				break;
				
			case EV_REL:
				printf("rel = %d, value = %d\n", event_buf[i].code, event_buf[i].value);
				break;
				
			default:
				break;	
		}
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
    struct epoll_event *pitem;
    int epoll_event_cnt;
	struct epoll_event pending_event_items[EPOLL_MAX_EVENTS];	
	int scan_enable = 0;
	
	printf("argc = %d, argv[0] = %s\n", argc, argv[0]);
	
	if (argc > 1) {
		scan_enable = atoi(argv[1]);
	}

	g_epoll_fd = epoll_create(EPOLL_SIZE_HINT);

    g_inotify_fd = inotify_init();
    result = inotify_add_watch(g_inotify_fd, DEVICE_PATH, IN_DELETE | IN_CREATE);
    printf("inotify_add_watch result = %d\n", result);
    
    if (scan_enable) {
		scan_device_dir(DEVICE_PATH);
	}
    
    memset(&event_item, 0, sizeof(event_item));
    event_item.events = EPOLLIN;
    event_item.data.u32 = EPOLL_ID_INOTIFY;
    result = epoll_ctl(g_epoll_fd, EPOLL_CTL_ADD, g_inotify_fd, &event_item);	
	while (1)
	{
		result = epoll_wait(g_epoll_fd, pending_event_items, EPOLL_MAX_EVENTS, -1);
		if (result <= 0) {
			if (errno != EINTR) {
				usleep(100000);
			}
			epoll_event_cnt = 0;
		} else {
			epoll_event_cnt = result;
		}
		
		for (i = 0; i < epoll_event_cnt; i++) {
			pitem = &pending_event_items[i];
			if (pitem->data.u32 == EPOLL_ID_INOTIFY) {
				fprintf(stderr, "============= [0x%x] ============\n", pending_event_items[i].data.u32);
				read_notify_event();	
				fprintf(stderr, "=================================\n");	
			} else {				
				input_device_t *pdev;
				
				pdev = (input_device_t *)pitem->data.u32;
				if (pdev) {
					read_input_event(pdev->fd);
				}
			}
		}
		
		
	}
	
	close(g_inotify_fd);
	close(g_epoll_fd);
	
	return 0;
} 

 
