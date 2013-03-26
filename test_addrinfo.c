/**
 *	@filename :
 * 		test_addrinfo.c
 **/
#include <stdio.h>
#include <string.h>  
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netdb.h>  

/*
 */
int addrinfo(char* host, char *serv);  
int addrinfo_ntop(const struct addrinfo *paddrinfo,  
                       char *host, size_t hostlen,  
                       char *serv, size_t servlen);
 
/*
 * 	./test_addrinfo -h www.example.com -p 8000
 */
int main(int argc, char *argv[]) {
	int ret;
	char *host = NULL;
	char *serv = NULL;
	char host_buf[64];
	char serv_buf[64];
    int result;

    opterr = 0; //使getopt不行stderr输出错误信息
	while( (result = getopt(argc, argv, "h:p:")) != -1 ) {
		switch(result) {
			case 'h':
				//printf("option=h, optopt=%c, optarg=%s\n", optopt, optarg);
				strcpy(host_buf, optarg);
				host = host_buf;
				break;
			case 'p':
				//printf("option=p, optopt=%c, optarg=%s\n", optopt, optarg);
				strcpy(serv_buf, optarg);
				serv = serv_buf;
				break;
			case '?':
				//printf("result=?, optopt=%c, optarg=%s\n", optopt, optarg);
				break;
			default:
				//printf("default, result=%c\n",result);
				break;
		}
		//printf("argv[%d]=%s\n", optind, argv[optind]);
	}
	
	printf("== check %s:%s ==\n", host, serv);
	if (!serv) {
		serv = "8000";
	}
    	
	ret = addrinfo(host, serv);
	
	printf("====== end check ======\n");
	
	return 0;
}

int addrinfo(char* host, char *serv)  {  
    struct addrinfo hints, *res = NULL, *paddrinfo = NULL;  
    char hostname[64]={0}, servname[64]={0};  
    int ret = -1;  
  
    //if( !host || !serv ) {
    if( !serv ) {
        return -1;
	}
	
    memset(&hints, 0, sizeof(hints));  
    hints.ai_family = AF_UNSPEC; /* AF_INET6 or AF_INET */  
    hints.ai_socktype = SOCK_DGRAM;  
    if ( !host ) {
		hints.ai_flags = AI_PASSIVE;  
	}
      
    if( (ret = getaddrinfo( host, serv, &hints, &res)) ) {  
        printf("getaddrinfo error:%s\n", gai_strerror(ret) );  
        return -1;  
    }  
          
    for(paddrinfo = res; paddrinfo; paddrinfo = paddrinfo->ai_next ) {  
        if( addrinfo_ntop(paddrinfo,  
                    hostname, sizeof(hostname),  
                    servname, sizeof(servname)) == 0 ) {  
            ret = 0;  
            fprintf(stderr, "addrinfo_ntop host/port = %s/%s\t", hostname, servname);  
        }  
          
        if( getnameinfo(paddrinfo->ai_addr, paddrinfo->ai_addrlen,   
                    hostname, sizeof(hostname),   
                    servname, sizeof(servname),   
                    NI_NUMERICHOST | NI_NUMERICSERV) == 0 ) {  
            ret = 0;  
            fprintf(stderr, "getnameinfo host/port = %s/%s\n", hostname, servname);  
        }  
    }  
  
    putchar('\n');  
      
    freeaddrinfo(res);  
    
    return ret;  
      
}  
  
int addrinfo_ntop(const struct addrinfo *paddrinfo,  
                       char *host, size_t hostlen,  
                       char *serv, size_t servlen) {  
    struct sockaddr_in *sain = NULL;  
    struct sockaddr_in6 *sain6 = NULL;  
  
    if( !paddrinfo || !host || !serv )  {
        return -1;  
	}
        
    switch ( paddrinfo->ai_family) {  
        case AF_INET:   
        {  
            sain = (struct sockaddr_in *)(paddrinfo->ai_addr);  
            inet_ntop( paddrinfo->ai_family, &(sain->sin_addr), host, hostlen);  
            snprintf( serv, servlen, "%d", ntohs(sain->sin_port) );  
            return 0;  
        }  
        break;  
  
        case AF_INET6:   
        {  
            sain6 = (struct sockaddr_in6 *)(paddrinfo->ai_addr);  
            inet_ntop( paddrinfo->ai_family, &(sain6->sin6_addr), host, hostlen);  
            snprintf( serv, servlen, "%d", ntohs(sain6->sin6_port) );  
            return 0;  
        };  
        break;  
  
        default:  
        {  
            return -1;  
            break;  
        }  
    }  
}
