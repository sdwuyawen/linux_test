/**
 * 	@filename :
 * 		sock_br_clinet.c
 * 
 * 	@note :
 * 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#define BUF_SIZE			1024

static int g_listen_sock = -1;

int main (void)
{
	struct sockaddr_in in_addr;
	socklen_t alen;
	char buf[BUF_SIZE];
	
	g_listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (g_listen_sock < 0) {
		printf("socket() error :%s\n", strerror(errno));
		return -1;
	}

	memset(&in_addr, 0, sizeof(in_addr));
	in_addr.sin_family = AF_INET;
	in_addr.sin_port = htons(5588);
	inet_aton("127.0.0.1", &in_addr.sin_addr);
	bzero(&in_addr.sin_zero, 8);
	alen = sizeof(struct sockaddr);
	
	if (connect(g_listen_sock, (struct sockaddr *) &in_addr, alen) < 0) {
		printf("connect error!\n");
		close(g_listen_sock);
		return -1;
	}		
	
	while (1) {
        fd_set read_fds;
        int rc = 0;
        int max = -1;

        FD_ZERO(&read_fds);
		
		max = g_listen_sock;
		FD_SET(g_listen_sock, &read_fds);

        if ((rc = select(max + 1, &read_fds, NULL, NULL, NULL)) < 0) {
            if (errno == EINTR)
                continue;
            printf("select fail\n");
            sleep(1);    
            continue;
        } else if (!rc)
            continue;
            
        if (FD_ISSET(g_listen_sock, &read_fds)) {
			rc = read(g_listen_sock, buf, BUF_SIZE);
			if (rc > 0) {
				buf[rc] = 0;
				printf("%s", buf);
			} else if (rc < 0) {
				break;
			}
		}		
	}
	
	close(g_listen_sock);
	
	return 0;
}
