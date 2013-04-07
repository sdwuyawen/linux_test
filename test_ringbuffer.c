/**
   @filename :
   		test_buffer.c
**/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>

#include <pthread.h>

#define BUF_USER_ID 	0x00
#define BUF_SERVER_ID	0x01

#define BUF_FRAME_COUNT 4096
#define BUF_FRAME_SIZE  0x01

#define BUF_FRAME_REQ	1600
#define BUF_FRAME_REQ_S	2048

#define DELAY_C			20
#define DELAY_S			100

/**
 **/
typedef struct {
	void *buffer;
	volatile uint32_t user;
	volatile uint32_t server;
	uint32_t user_base;
	uint32_t server_base;
	uint32_t frame_count;
	uint32_t frame_size;
	pthread_mutex_t lock;
	pthread_cond_t cond;
}buffer_cblk_t;

/**
 **/
static buffer_cblk_t g_buffer_cblk;
static int g_client_runnig = 1;

/**
 **/
static void *get_user_buffer(buffer_cblk_t *pcblk, uint32_t offset) {	
	return (int8_t *)pcblk->buffer + (pcblk->user - pcblk->user_base) * pcblk->frame_size;
}

static uint32_t frames_available(buffer_cblk_t *pcblk) {
	return (pcblk->server + pcblk->frame_count - pcblk->user);
}

static uint32_t frames_ready(buffer_cblk_t *pcblk) {
	return (pcblk->user - pcblk->server);
}

static uint32_t step_user(buffer_cblk_t *pcblk, uint32_t frames) {
	uint32_t u;
	uint32_t fc = pcblk->frame_count;
	
	u = pcblk->user;
	u += frames;
	if (u >= fc) {
		if (u - fc >= pcblk->user_base) {
			/* common case, user don`t just wrap */
			pcblk->user_base += fc;
		}
	} else if (u >= pcblk->user_base + fc) {
		//user just wrapped
		pcblk->user_base += fc;
	}
		
	pcblk->user = u;

	return u;
}

static int step_server(buffer_cblk_t *pcblk, uint32_t frames) {
	uint32_t s;
	uint32_t fc = pcblk->frame_count;

	pthread_mutex_lock(&pcblk->lock);
	
	s = pcblk->server;
	s += frames;
	if (s >= fc) {
		// common case, server didn't just wrap
		if (s - fc >= pcblk->server_base ) {
			pcblk->server_base += fc;
		}
	} else if (s >= pcblk->server_base + fc) {
		// server just wrapped
		pcblk->server_base += fc;
	}

	pcblk->server = s;

	pthread_cond_signal(&pcblk->cond);
	
	pthread_mutex_unlock(&pcblk->lock);
			
	return 0;
}

static void *thread_client(void *pdata) {
	buffer_cblk_t *pcblk = (buffer_cblk_t *)pdata;
	uint32_t frames_avail;
	uint32_t frames_req;
	void *ptr;
	
	while (g_client_runnig) {
	    pthread_mutex_lock(&pcblk->lock);
		frames_avail = frames_available(pcblk);
		if (frames_avail == 0) {
			printf("[c]wait server\n");
			pthread_cond_wait(&pcblk->cond, &pcblk->lock);
			
			if (g_client_runnig == 0) {
				break;
			}			
			frames_avail = frames_available(pcblk);			
		}
		pthread_mutex_unlock(&pcblk->lock);

		frames_req = BUF_FRAME_REQ;
		if (frames_req > frames_avail) {
			frames_req = frames_avail;
		}
		uint32_t u = pcblk->user;
		uint32_t buffer_end = pcblk->user_base + pcblk->frame_count;
		
		printf("[c]avail=0x%x u=0x%x, end=0x%x\n", frames_avail, u, buffer_end);
		if (frames_req > buffer_end - u) {
			frames_req = buffer_end - u;
		}

		if (u < pcblk->user_base) {
			printf("[c] u < user_base\n");
			g_client_runnig = 0;
			break;
		}
		
		ptr = get_user_buffer(pcblk, u);
		memset(ptr, 0, frames_req*pcblk->frame_size);
		printf("[c]buf = 0x%x, size = 0x%x\n", (unsigned int)ptr, frames_req*pcblk->frame_size);
		
		step_user(pcblk, frames_req);
				
		usleep(DELAY_C*1000);
	}
	
	return NULL;
}

/**
 **/
int main(int argc, char *argv[]) {
	pthread_t tid;
	int is_running = 1;
	uint32_t frames_rdy;
	uint32_t frames_req;
	buffer_cblk_t *pcblk = &g_buffer_cblk;
	void *ptr;
	uint32_t delay = -1;
	uint32_t cnt;

	if (argc > 1) {
		delay = atoi(argv[1]);
	}
	
	pthread_mutex_init(&pcblk->lock, NULL);
	pthread_cond_init(&pcblk->cond, NULL);

	memset(pcblk, 0, sizeof(*pcblk));
	pcblk->buffer = malloc(BUF_FRAME_COUNT*BUF_FRAME_SIZE);
	pcblk->frame_count = BUF_FRAME_COUNT;
	pcblk->frame_size = BUF_FRAME_SIZE;

	printf("[init] buf = 0x%x, end = 0x%x\n", (unsigned int)pcblk->buffer, (unsigned int)pcblk->buffer + BUF_FRAME_SIZE*BUF_FRAME_COUNT);

	pthread_create(&tid, NULL, thread_client, pcblk);

	cnt = 0;
	while (is_running && g_client_runnig) {
		frames_rdy = frames_ready(pcblk);
		if (frames_rdy == 0) {
			usleep(100);
			continue;
		}

		frames_req = BUF_FRAME_REQ_S;
		uint32_t s = pcblk->server;
		uint32_t buffer_end = pcblk->server_base + pcblk->frame_count;

		if (frames_req > frames_rdy) {
			frames_req = frames_rdy;
		}
		
		if (frames_req > buffer_end - s) {
			frames_req = buffer_end - s;
		}
		
		ptr = (int8_t *)pcblk->buffer + (s - pcblk->server_base) * pcblk->frame_size;
		memset(ptr, 0, frames_req*pcblk->frame_size);
		printf("[s]buf = 0x%x, size = 0x%x\n", (unsigned int)ptr, frames_req*pcblk->frame_size);
		step_server(pcblk, frames_req);
		
		usleep(DELAY_S*1000);
		cnt++;
		if (cnt >= (1000/DELAY_S)) {
			cnt = 0;
			delay--;
			if (delay == 0) {
				printf("exit main loop\n");
				is_running = 0;
			}
		}
	}

	g_client_runnig = 0;
	pthread_cond_signal(&pcblk->cond);
	pthread_join(tid, NULL);

	pthread_cond_destroy(&pcblk->cond);
	pthread_mutex_destroy(&pcblk->lock);

	if (pcblk->buffer) {
		free(pcblk->buffer);
		pcblk->buffer = NULL;
	}
    
    return 0;
}


