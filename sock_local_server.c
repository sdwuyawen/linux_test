/**
 * 	@filename :
 * 		sock_local_server.c
 * 
 * 	@note :
 * 	
 **/
//s_unix.c
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h> 
#include <sys/wait.h>

#define UNIX_DOMAIN "/tmp/UNIX.domain"

int main(void)
{
    socklen_t clt_addr_len;
    int listen_fd;
    int com_fd;
    int ret;
    int i;
    static char recv_buf[1024]; 
    int len;
    struct sockaddr_un clt_addr;
    struct sockaddr_un srv_addr;
    fd_set cur_rfds, glb_rfds;
    int maxfd;
    
    FD_ZERO(&cur_rfds);
    FD_ZERO(&glb_rfds);
    
    signal(SIGPIPE, SIG_IGN); 
    
    listen_fd = socket(PF_UNIX, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("cannot create communication socket");
        return 1;
    }  
    
    //set server addr_param
    srv_addr.sun_family = AF_UNIX;
    strncpy(srv_addr.sun_path, UNIX_DOMAIN, sizeof(srv_addr.sun_path)-1);
    unlink(UNIX_DOMAIN);
    //bind sockfd & addr
    ret = bind(listen_fd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
    if (ret == -1) {
        perror("cannot bind server socket");
        close(listen_fd);
        unlink(UNIX_DOMAIN);
        return 1;
    }
    
    //listen sockfd 
    ret = listen(listen_fd, 1);
    if (ret == -1) {
        perror("cannot listen the client connect request");
        close(listen_fd);
        unlink(UNIX_DOMAIN);
        return 1;
    }
    
    maxfd = listen_fd;
    FD_SET(listen_fd, &glb_rfds);
    com_fd = 0;
    //have connect request use accept
    while (1) {
       cur_rfds = glb_rfds;
       if(select(maxfd+1, &cur_rfds, NULL, NULL, 0) < 0) {
           perror("fail to select");
           exit(1);
       }
		
       if (FD_ISSET(listen_fd, &cur_rfds)) {
			len = sizeof(clt_addr);
			com_fd = accept(listen_fd, (struct sockaddr*)&clt_addr, &len);
			if (com_fd<0) {
				perror("cannot accept client connect request");
				break;
			}
			
			FD_SET(com_fd, &glb_rfds);
			if (com_fd > maxfd) {
				maxfd = com_fd;
			}
			printf("[s]input fd = %d\n", com_fd);
		}
		
		if (com_fd > 0) {
			if (FD_ISSET(com_fd, &cur_rfds)) {
				//read and printf sent client info
				printf("[s]\n=====info=====\n");
				memset(recv_buf, 0, 1024);
				int num = read(com_fd, recv_buf, sizeof(recv_buf));
				if (num <= 0) {
					printf("[s]error client %d\n", com_fd);
					close(com_fd);
					FD_CLR(com_fd, &glb_rfds);
					com_fd = 0;
					maxfd = listen_fd;					
					//break;
				}
				
				if (num > 0) {
					printf("[s]Message from client (%d)) :%s\n", num, recv_buf);  
				} 
			}    			
		}
	}
	
	if (com_fd > 0) {
		close(com_fd);
	}
    close(listen_fd);
    unlink(UNIX_DOMAIN);
    
    printf("[s]exit sock_local_server\n");
    
    return 0;
}

