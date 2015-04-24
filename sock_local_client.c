/**
 * 	@filename :
 * 		sock_local_client.c
 * 
 * 	@note :
 * 	
 **/
//c_unix.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>

#define UNIX_DOMAIN "/tmp/UNIX.domain"

static int connect_fd;

static void signal_handler(int num)
{
    printf("Capture signal no:%d\n",num); 
    if (connect_fd > 0) {
		close(connect_fd);
		connect_fd = 0;
		printf("exit sock_local_client for signal\n");
		exit(1);
	}
}
     
int main(void)
{
    int ret;
    char snd_buf[1024];
    int i;
    static struct sockaddr_un srv_addr;
    
    signal(SIGPIPE, SIG_IGN); 
    signal(SIGINT, signal_handler); 
    
	//creat unix socket
    connect_fd = socket(PF_UNIX, SOCK_STREAM, 0);
    if (connect_fd < 0) {
        perror("cannot create communication socket");
        return 1;
    }   
    
    srv_addr.sun_family=AF_UNIX;
    strcpy(srv_addr.sun_path,UNIX_DOMAIN);
	//connect server
    ret = connect(connect_fd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
    if (ret == -1) {
        perror("cannot connect to the server");
        close(connect_fd);
        return 1;
    }
    
    memset(snd_buf, 0, 1024);
    strcpy(snd_buf, "message from client");
//send info server
    //for (i=0; i<4; i++) {
    while (1) {
		sleep(1);
        ret = write(connect_fd, snd_buf, sizeof(snd_buf));
        if (ret < 0) {
			break;
		}
	}
	
    close(connect_fd);
    printf("exit sock_local_client\n");
    
    return 0;
}
