/**
 *	@filename :
 * 		test_net.c
 *  
 *  @note :
 * 		测试网络event
 * 
 **/
#include <stdio.h>
#include <errno.h>
#include <string.h>
 
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <linux/if.h>

#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#define BUFLEN 20480  
 
static int setupSocket(int *sock, int netlinkFamily, int groups, int format) 
{

    struct sockaddr_nl nladdr;
    int sz = 64 * 1024;
    int on = 1;

    memset(&nladdr, 0, sizeof(nladdr));
    nladdr.nl_family = AF_NETLINK;
    nladdr.nl_pid = getpid();
    nladdr.nl_groups = groups;

    if ((*sock = socket(PF_NETLINK, SOCK_DGRAM, netlinkFamily)) < 0) {
        printf("Unable to create netlink socket: %s", strerror(errno));
        return -1;
    }

    if (setsockopt(*sock, SOL_SOCKET, SO_RCVBUFFORCE, &sz, sizeof(sz)) < 0) {
        printf("Unable to set uevent socket SO_RCVBUFFORCE option: %s", strerror(errno));
        close(*sock);
        return -1;
    }

    if (setsockopt(*sock, SOL_SOCKET, SO_PASSCRED, &on, sizeof(on)) < 0) {
        printf("Unable to set uevent socket SO_PASSCRED option: %s", strerror(errno));
        close(*sock);
        return -1;
    }

    if (bind(*sock, (struct sockaddr *) &nladdr, sizeof(nladdr)) < 0) {
        printf("Unable to bind netlink socket: %s", strerror(errno));
        close(*sock);
        return -1;
    }

	return 0;
}

int main(int argc, char *argv[])
{	
    int fd, retval;  
    char buf[BUFLEN] = {0};  
    int len = BUFLEN;  
    struct sockaddr_nl addr;  
    struct nlmsghdr *nh;  
    struct ifinfomsg *ifinfo;  
    struct rtattr *attr;  
    	
	setupSocket(&fd, NETLINK_ROUTE, RTMGRP_LINK, 0);
    //setupSocket(&fd, NETLINK_NFLOG, 1, 0);     
        
    while ((retval = read(fd, buf, BUFLEN)) > 0)  
    {  
        for (nh = (struct nlmsghdr *)buf; NLMSG_OK(nh, retval); nh = NLMSG_NEXT(nh, retval))  
        {  
            if (nh->nlmsg_type == NLMSG_DONE)  
                break;  
            else if (nh->nlmsg_type == NLMSG_ERROR)  
                return;  
            else if (nh->nlmsg_type != RTM_NEWLINK)  
                continue;  
            ifinfo = NLMSG_DATA(nh);  
            printf("%u: %s", ifinfo->ifi_index,  
                    (ifinfo->ifi_flags & IFF_LOWER_UP) ? "up" : "down" );  
            attr = (struct rtattr*)(((char*)nh) + NLMSG_SPACE(sizeof(*ifinfo)));  
            len = nh->nlmsg_len - NLMSG_SPACE(sizeof(*ifinfo));  
            for (; RTA_OK(attr, len); attr = RTA_NEXT(attr, len))  
            {  
                if (attr->rta_type == IFLA_IFNAME)  
                {  
                    printf(" %s", (char*)RTA_DATA(attr));  
                    break;  
                }  
            }  
            printf("\n");  
        }  
    }  
    
    close(fd);
             
	return 0;
}
