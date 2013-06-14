/**
 * @filename :
 * 		test_event.c
 * 
 * @note :
 * 		
 * 
 **/
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <malloc.h>

#include <stdbool.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include <linux/netlink.h>

#ifndef ANDROID_OS
#define SLOGE		printf
#else
#endif

//#define CREDENTIALS_ENABLE

#define BUF_SZ		(64*1024)

/**
 * Like recv(), but checks that messages actually originate from the kernel.
 */
ssize_t uevent_kernel_multicast_recv(int socket, void *buffer, size_t length) {
#if 1
    struct iovec iov = { buffer, length };
    struct sockaddr_nl addr;
    
#ifdef CREDENTIALS_ENABLE    
    char control[CMSG_SPACE(sizeof(struct ucred))];
#endif    
    struct msghdr hdr = {
        &addr,
        sizeof(addr),
        &iov,
        1,
#ifdef CREDENTIALS_ENABLE            
        control,
        sizeof(control),
#endif    
        0,
    };

    ssize_t n = recvmsg(socket, &hdr, 0);
    if (n <= 0) {
        return n;
    }

    if (addr.nl_groups == 0 || addr.nl_pid != 0) {
        /* ignoring non-kernel or unicast netlink message */
        goto out;
    }
    
#ifdef CREDENTIALS_ENABLE    
    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&hdr);
    if (cmsg == NULL || cmsg->cmsg_type != SCM_CREDENTIALS) {
        /* ignoring netlink message with no sender credentials */
        goto out;
    }

    struct ucred *cred = (struct ucred *)CMSG_DATA(cmsg);
    if (cred->uid != 0) {
        /* ignoring netlink message from non-root user */
        goto out;
    }
#endif
    return n;

out:
    /* clear residual potentially malicious data */
    bzero(buffer, length);
    errno = EIO;
    return -1;
#else
	return read(socket, buffer, length);
#endif    
}

int uevent_open_socket(int buf_sz, bool passcred)
{
    struct sockaddr_nl addr;
    int on = passcred;
    int s;

    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_pid = getpid();
    addr.nl_groups = 0xffffffff;

    s = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
    if(s < 0)
        return -1;

    setsockopt(s, SOL_SOCKET, SO_RCVBUFFORCE, &buf_sz, sizeof(buf_sz));
    setsockopt(s, SOL_SOCKET, SO_PASSCRED, &on, sizeof(on));

    if(bind(s, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        close(s);
        return -1;
    }

    return s;
}
 
int main(int argc, char *argv[]){	
	void *buffer;
	int sock = -1;
	
	buffer = malloc(BUF_SZ + 1024);
	if (buffer == NULL) {
		printf("malloc buffer error!\n");
		return -1;
	}
	
	sock = uevent_open_socket(BUF_SZ, false);
	if (sock < 0) {
		printf("uevent open socket error!\n");
		free(buffer);
		return -1;
	}
	
	while (1) {
        fd_set read_fds;
        int rc = 0;
        int max = -1;

        FD_ZERO(&read_fds);

        max = sock;
        FD_SET(sock, &read_fds);
        
        if ((rc = select(max + 1, &read_fds, NULL, NULL, NULL)) < 0) {
            if (errno == EINTR)
                continue;
            SLOGE("select failed (%s)", strerror(errno));
            sleep(1);
            continue;
        } else if (!rc)
            continue;

        if (FD_ISSET(sock, &read_fds)) {
			ssize_t sz;
			
			memset(buffer, 0, BUF_SZ);
			sz = uevent_kernel_multicast_recv(sock, buffer, BUF_SZ);
			if (sz > 0) {
				printf("%s\n", (char *)buffer);
			}
		}
	}
	
	close(sock);
	free(buffer);
	
	return 0;
}
