/**
 * @filename :
 * 		test_event.c
 * 
 * @note :
 * 		
 * 
 **/   
#include <stdio.h> 
#include <malloc.h>

#include <errno.h>
#include <stdbool.h>
#include <string.h>
#define __USE_GNU
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <linux/netlink.h>

#ifndef ANDROID_OS
#define SLOGE		printf
#else
#endif

#ifdef __USE_GNU
#define CREDENTIALS_ENABLE
#endif 

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
        printf("no sender credentials\n");
        goto out;
    }

    struct ucred *cred = (struct ucred *)CMSG_DATA(cmsg);
    if (cred->uid != 0) {
        /* ignoring netlink message from non-root user */
        printf("non-root user\n");
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

/* Same as strlen(x) for constant string literals ONLY */
#define CONST_STRLEN(x)  (sizeof(x)-1)

/* Convenience macro to call has_prefix with a constant string literal  */
#define HAS_CONST_PREFIX(str,end,prefix)  has_prefix((str),(end),prefix,CONST_STRLEN(prefix))

/*
 * Parse an ASCII-formatted message from a NETLINK_KOBJECT_UEVENT
 * netlink socket.
 */
bool uevent_parse_message(char *buffer, int size) {
    const char *s = buffer;
    int i;
    const char *p;

    if (size == 0)
        return false;
	
    /* Ensure the buffer is zero-terminated, the code below depends on this */
    buffer[size-1] = '\0';
    	
	/* buffer is 0-terminated, no need to check p < end */
	for (p = s; *p != '@'; p++) {
		if (!*p) { /* no '@', should not happen */
			return false;
		}
	}
	buffer[p - s] = 0;

	if (strstr(p+1, "block") == NULL) {
		//ignore not block event
		return false;
	}
            
    if (!strcmp(s, "add")) {
		printf("-> add ");
	} else if (!strcmp(s, "remove")) {
		printf("-> remove ");
	} else if (!strcmp(s, "change")) {
		printf("-> change ");
	}

	printf(" %s\n", p+1);
    
    return true;
}
 
int main(int argc, char *argv[]){	
	void *buffer;
	int sock = -1;
	
	buffer = malloc(BUF_SZ + 1024);
	if (buffer == NULL) {
		printf("malloc buffer error!\n");
		return -1;
	}
	
	sock = uevent_open_socket(BUF_SZ, true);
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
				uevent_parse_message(buffer, sz);
			}
		}
	}
	
	close(sock);
	free(buffer);
	
	return 0;
}
