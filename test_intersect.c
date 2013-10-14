/**
 *	@filename :
 * 		test_intersect.c
 * 
 *  @note :
 * 
 **/
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#define TEST_RECTS_MAX		16
#define TEST_BUFFER_SIZE	256

typedef struct {
	int left, top;
	int right, bottom;	
}test_rect_t;

#ifndef max
	#define max(a, b)	(a>b?a:b)
	#define min(a, b)	(a<b?a:b)
#endif 

/*
 * 
 */
static int g_rect_num;
static test_rect_t g_src_rects[TEST_RECTS_MAX];

static int is_intersect(test_rect_t *src, test_rect_t *dst)
{
	return !(src->left > dst->right || src->right < dst->left ||
		src->top > dst->bottom || src->bottom < dst->top);
}

static int intersect(test_rect_t *src, test_rect_t *dst, test_rect_t *out)
{	
	if(is_intersect(src, dst))
	{
		out->left = max(src->left, dst->left);
		out->top = max(src->top, dst->top);
		out->right = min(src->right, dst->right);
		out->bottom = min(src->bottom, dst->bottom);
		
		return 1;
	}

	return 0;
}

/*
 *
 */
static int check_intersect(int index) 
{
	test_rect_t rect;
	int i;
	int ret;
	
	if (index > 0) {
		for (i=0; i<index; i++) {
			memset(&rect, 0, sizeof(rect));
			
			ret = intersect(&g_src_rects[index], &g_src_rects[i], &rect);
			printf("\t[%02d] (%d, %d, %d, %d)\n",
				i,
				rect.left, rect.top,
				rect.right, rect.bottom);
		}
	}
	
	return 0;
}

/*
 *
 */ 
int main(int argc, char *argv[])
{	
	FILE *pf = NULL;
	char buffer[TEST_BUFFER_SIZE];
	int i;
	int w, h;
	
	if (argc != 2) {
		printf("%s rect.lst", argv[0]);
		return -1;
	}
	
	pf = fopen(argv[1], "r");
	if (pf == NULL) {
		printf("can not open file %s\n", argv[1]);
		return -1;
	}
	
	memset(g_src_rects, 0, sizeof(g_src_rects));
	g_rect_num = 0;
	while (fgets(buffer, TEST_BUFFER_SIZE, pf)) {
		if (buffer[0] == '#') {
			continue;
		}
		
		g_src_rects[g_rect_num].left = atoi(strtok(buffer, ","));
		g_src_rects[g_rect_num].top = atoi(strtok(NULL, ","));
		g_src_rects[g_rect_num].right = g_src_rects[g_rect_num].left + atoi(strtok(NULL, ",")) - 1;
		g_src_rects[g_rect_num].bottom = g_src_rects[g_rect_num].top + atoi(strtok(NULL, ",")) - 1;
		
		g_rect_num++;
		if (g_rect_num == TEST_RECTS_MAX) {
			break;
		}
	}	
	fclose(pf);
	
	if (g_rect_num > 1) {
		printf("Check intersections ...\n");
		printf("=============================\n");
		printf("(left, top, right, bottom)\n");
		printf("=============================\n");
		for (i=0; i<g_rect_num; i++) {
			printf("[%02d] (%d, %d, %d, %d)",
				i,
				g_src_rects[i].left, g_src_rects[i].top,
				g_src_rects[i].right, g_src_rects[i].bottom);
			printf(" : \n");
			//
			check_intersect(i);
			//			
			printf("\n");	
		}
		printf("=============================\n");
		printf("Check intersections finished!\n");
	}
	
	return 0;
}
