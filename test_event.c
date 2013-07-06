/**
 *	@filename :
 * 		test_event.c
 * 
 * 	@note :
 * 		(1).recvmsg()函数会读取多个字符串返回到msg中，要分析msg中的多个字符串的信息才行。
 * 
 **/ 
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <sys/types.h>
#include <sys/un.h>

#define _CRED_EN_

struct uevent {
    const char *action;
    const char *path;
    const char *subsystem;
    const char *firmware;
    int major;
    int minor;
};

static int open_uevent_socket(void)
{
    struct sockaddr_nl addr;
    int sz = 64*1024; // XXX larger? udev uses 16MB!
    int on = 1;
    int s;

    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_pid = getpid();
    addr.nl_groups = 0xffffffff;

    s = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
    if(s < 0)
        return -1;

    setsockopt(s, SOL_SOCKET, SO_RCVBUFFORCE, &sz, sizeof(sz));
    setsockopt(s, SOL_SOCKET, SO_PASSCRED, &on, sizeof(on));

    if(bind(s, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        close(s);
        return -1;
    }

    return s;
}

static void parse_event(const char *msg, struct uevent *uevent)
{
#if  1
    uevent->action = "";
    uevent->path = "";
    uevent->subsystem = "";
    uevent->firmware = "";
    uevent->major = -1;
    uevent->minor = -1;

        /* currently ignoring SEQNUM */
    while(*msg) {
		printf("%s\n", msg);
        if(!strncmp(msg, "ACTION=", 7)) {
            msg += 7;
            uevent->action = msg;
        } else if(!strncmp(msg, "DEVPATH=", 8)) {
            msg += 8;
            uevent->path = msg;
        } else if(!strncmp(msg, "SUBSYSTEM=", 10)) {
            msg += 10;
            uevent->subsystem = msg;
        } else if(!strncmp(msg, "FIRMWARE=", 9)) {
            msg += 9;
            uevent->firmware = msg;
        } else if(!strncmp(msg, "MAJOR=", 6)) {
            msg += 6;
            uevent->major = atoi(msg);
        } else if(!strncmp(msg, "MINOR=", 6)) {
            msg += 6;
            uevent->minor = atoi(msg);
        }

            /* advance to after the next \0 */
        while(*msg++)
            ;
    }

    printf("event { '%s', '%s', '%s', '%s', %d, %d }\n",
                    uevent->action, uevent->path, uevent->subsystem,
                    uevent->firmware, uevent->major, uevent->minor);
#endif
}

#define UEVENT_MSG_LEN  1024
void handle_device_fd(int fd)
{
	printf("enter %s\n", __func__);
    for(;;) {
        char msg[UEVENT_MSG_LEN+2];
    #ifdef _CRED_EN_
        char cred_msg[CMSG_SPACE(sizeof(struct ucred))];
    #endif
        struct iovec iov = {msg, sizeof(msg)};
        struct sockaddr_nl snl;
    #ifdef _CRED_EN_
        struct msghdr hdr = {&snl, sizeof(snl), &iov, 1, cred_msg, sizeof(cred_msg), 0};
	#else
		struct msghdr hdr = {&snl, sizeof(snl), &iov, 1, 0};
	#endif
        ssize_t n = recvmsg(fd, &hdr, 0);
        if (n <= 0) {
            break;
        }

        if ((snl.nl_groups == 0) || (snl.nl_pid != 0)) {
            /* ignoring non-kernel netlink multicast message */
            continue;
        }
	#ifdef _CRED_EN_
        struct cmsghdr * cmsg = CMSG_FIRSTHDR(&hdr);
        if (cmsg == NULL || cmsg->cmsg_type != SCM_CREDENTIALS) {
            /* no sender credentials received, ignore message */
            continue;
        }
		
        struct ucred * cred = (struct ucred *)CMSG_DATA(cmsg);
        if (cred->uid != 0) {
            /* message from non-root user, ignore */
            continue;
        }
	#endif
        if(n >= UEVENT_MSG_LEN)   /* overflow -- discard */
            continue;

        msg[n] = '\0';
        msg[n+1] = '\0';

        struct uevent uevent;
        parse_event(msg, &uevent);
	}
}

int main(void)
{
	int fd = 0;
	
	fd = open_uevent_socket();
	if (fd < 0) {
		printf("error!\n");
		return -1;
	}
	
	handle_device_fd(fd);
}
