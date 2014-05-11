/**
 * 	@filename :
 * 		sock_br_server.c
 * 
 * 	@note :
 * 		 1.执行：
 * 			server: adb logcat | ./bin/sock_br_server
 * 			client: ./bin/sock_br_client
 * 
 * 		 2.增加SIGPIPE处理
 * 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>

//#define STANDARD_IN			fd[0]
#define STANDARD_IN			0

#define BUF_SIZE			1024

typedef struct _sock_br_client_t {
	struct _sock_br_client_t *next;
	int fd;
}sock_br_client_t;

static sock_br_client_t *g_clients_head = NULL;
static int g_clients_cnt = 0;
static int g_listen_sock = -1;

static void handle_pipe(int sig)
{
	//不做任何处理即可
}

int main (void)
{
	char buf[BUF_SIZE];
	sock_br_client_t *pclient = NULL;
	struct sockaddr_in in_addr;
	struct sockaddr addr;
	socklen_t alen;
	pid_t pid;
	int n;
	
#if 0	
	int fd[2];
	pipe(fd);
	
	pid = fork();
	if (pid == 0) {
		printf("adb client");
		//close(fd[0]);
		dup2(fd[1], 1);		//1 -> fd[1]
		//dup2(fd[1], 2);		//2 -> fd[1]
		execl("adb", "adb", "logcat", NULL);
		exit(1);
	}
	//close(fd[1]);
#endif
	
	g_listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (g_listen_sock < 0) {
		printf("socket() error :%s\n", strerror(errno));
		return -1;
	}
	
    n = 1;
    setsockopt(g_listen_sock, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(n));
    	
	memset(&in_addr, 0, sizeof(in_addr));
	in_addr.sin_family = AF_INET;
	in_addr.sin_port = htons(5588);
	in_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&in_addr.sin_zero, 8);
	alen = sizeof(struct sockaddr);
    	
	if (bind(g_listen_sock, (struct sockaddr *) &in_addr, alen) < 0) {
		printf("bind error!\n");
		close(g_listen_sock);
		return -1;
	}
	
	if (listen(g_listen_sock, 10) < 0) {
		printf("listen error!\n");
		close(g_listen_sock);
		return -1;		
	}
	
	struct sigaction action;
	
	action.sa_handler = handle_pipe;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGPIPE, &action, NULL);

    while(1) {
        fd_set read_fds;
        int rc = 0;
        int max = -1;

        FD_ZERO(&read_fds);
		
		max = STANDARD_IN;
		FD_SET(STANDARD_IN, &read_fds);
		if (max < g_listen_sock) {
			max = g_listen_sock;
		}
		FD_SET(g_listen_sock, &read_fds);

        if ((rc = select(max + 1, &read_fds, NULL, NULL, NULL)) < 0) {
            if (errno == EINTR)
                continue;
            printf("select fail\n");
            sleep(1);    
            continue;
        } else if (!rc)
            continue;
            
        //printf("select ok!\n");    
        if (FD_ISSET(g_listen_sock, &read_fds)) {
            int c;

            do {
                alen = sizeof(addr);
                c = accept(g_listen_sock, &addr, &alen);
                printf("got %d from accept\n", c);
            } while (c < 0 && errno == EINTR);
            
            if (c < 0) {
                printf("accept failed (%s)\n", strerror(errno));
                sleep(1);
                continue;
            }
           
            //add client	
            pclient = malloc(sizeof(sock_br_client_t)); 
            pclient->fd = c;
			pclient->next = NULL;
            if (g_clients_head) {
				pclient->next = g_clients_head;
			}
			g_clients_cnt++;
			g_clients_head = pclient;
        }
        
        if (FD_ISSET(STANDARD_IN, &read_fds)) { 
			int rd_ret;
			int wr_ret;
			sock_br_client_t *prev = NULL;
						
			rd_ret = read(STANDARD_IN, buf, BUF_SIZE);
			//printf("rd_ret = %d\n", rd_ret);
			if (rd_ret > 0) {
				//buf[rd_ret] = 0;
				//printf("%s", buf);
				for (pclient = g_clients_head; pclient != NULL;) {
					wr_ret = write(pclient->fd, buf, rd_ret);
					if (wr_ret < 0) {
						printf("delete %d\n", pclient->fd);
						//delete client
						if (prev) {
							prev->next = pclient->next;
						} else {
							g_clients_head = pclient->next;
						}
						close(pclient->fd);
						pclient = pclient->next;
						free(pclient);
						g_clients_cnt--;
					} else {
						prev = pclient;
						pclient = pclient->next;
					}
				}
			}
		}
    }	
    
    close(g_listen_sock);
    
	return 0;
}
