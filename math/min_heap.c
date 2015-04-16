/**
 * 	@filename :
 * 		min_heap.c
 * 
 * 	@note :
 *      堆是一种经过排序的完全二叉树，其中任一非终端节点的数据值均不大于（或不小于）其左孩子和右孩子节点的值
 * 		最大堆：根结点的键值是所有堆结点键值中最大者。
 * 		最小堆：根结点的键值是所有堆结点键值中最小者
 *
 **/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define HEAP_SIZE		8

typedef struct heap_item {
	int value;
	int min_heap_idx;
} heap_item_t;

typedef struct min_heap {
	heap_item_t **p;
	unsigned n, a;			//item number, alloc number
} min_heap_t;

#define HEAP_DEBUG

#define mm_free		free
#define mm_malloc 	malloc
#define mm_realloc	realloc

static inline void	     min_heap_ctor_(min_heap_t* s);
static inline void	     min_heap_dtor_(min_heap_t* s);
static inline void	     min_heap_elem_init_(heap_item_t *e);
static inline int	     min_heap_elt_is_top_(const heap_item_t *e);
static inline int	     min_heap_empty_(min_heap_t* s);
static inline unsigned	     min_heap_size_(min_heap_t* s);
static inline heap_item_t*  min_heap_top_(min_heap_t* s);
static inline int	     min_heap_reserve_(min_heap_t* s, unsigned n);
static inline int	     min_heap_push_(min_heap_t* s, heap_item_t *e);
static inline heap_item_t*  min_heap_pop_(min_heap_t* s);
static inline int	     min_heap_adjust_(min_heap_t *s, heap_item_t *e);
static inline int	     min_heap_erase_(min_heap_t* s, heap_item_t *e);
static inline void	     min_heap_shift_up_(min_heap_t* s, unsigned hole_index, heap_item_t *e);
static inline void	     min_heap_shift_up_unconditional_(min_heap_t* s, unsigned hole_index, heap_item_t *e);
static inline void	     min_heap_shift_down_(min_heap_t* s, unsigned hole_index, heap_item_t *e);

#define min_heap_elem_greater(a, b) \
	(a->value > b->value)

void min_heap_ctor_(min_heap_t* s) { s->p = 0; s->n = 0; s->a = 0; }
void min_heap_dtor_(min_heap_t* s) { if (s->p) mm_free(s->p); }
void min_heap_elem_init_(heap_item_t* e) { e->min_heap_idx = -1; }
int min_heap_empty_(min_heap_t* s) { return 0u == s->n; }
unsigned min_heap_size_(min_heap_t* s) { return s->n; }
heap_item_t* min_heap_top_(min_heap_t* s) { return s->n ? *s->p : 0; }

int min_heap_push_(min_heap_t* s, heap_item_t *e)
{
	if (min_heap_reserve_(s, s->n + 1)) {
		return -1;
	}
	
	min_heap_shift_up_(s, s->n++, e);
	
	return 0;
}

heap_item_t* min_heap_pop_(min_heap_t* s)
{
	if (s->n) {
		heap_item_t* e = *s->p;
		min_heap_shift_down_(s, 0u, s->p[--s->n]);
		e->min_heap_idx = -1;
		return e;
	}
	return 0;
}

int min_heap_elt_is_top_(const heap_item_t *e)
{
	return e->min_heap_idx == 0;
}

int min_heap_erase_(min_heap_t* s, heap_item_t* e)
{
	if (-1 != e->min_heap_idx) {
		heap_item_t *last = s->p[--s->n];
		unsigned parent = (e->min_heap_idx - 1) / 2;
		/* we replace e with the last element in the heap.  We might need to
		   shift it upward if it is less than its parent, or downward if it is
		   greater than one or both its children. Since the children are known
		   to be less than the parent, it can't need to shift both up and
		   down. */
		if (e->min_heap_idx > 0 && min_heap_elem_greater(s->p[parent], last)) {
			min_heap_shift_up_unconditional_(s, e->min_heap_idx, last);
		} else {
			min_heap_shift_down_(s, e->min_heap_idx, last);
		}
		e->min_heap_idx = -1;
		return 0;
	}
	return -1;
}

int min_heap_adjust_(min_heap_t *s, heap_item_t *e)
{
	if (-1 == e->min_heap_idx) {
		return min_heap_push_(s, e);
	} else {
		unsigned parent = (e->min_heap_idx - 1) / 2;
		/* The position of e has changed; we shift it up or down
		 * as needed.  We can't need to do both. */
		if (e->min_heap_idx > 0 && min_heap_elem_greater(s->p[parent], e)) {
			min_heap_shift_up_unconditional_(s, e->min_heap_idx, e);
		} else {
			min_heap_shift_down_(s, e->min_heap_idx, e);
		}
		return 0;
	}
}

int min_heap_reserve_(min_heap_t* s, unsigned n)
{
	if (s->a < n) {
		heap_item_t** p;
		unsigned a = s->a ? s->a * 2 : 8;
		
		if (a < n) {
			a = n;
		}
		
		if (!(p = (heap_item_t**)mm_realloc(s->p, a * sizeof *p))) {
			return -1;
		}
		s->p = p;
		s->a = a;
	}
	
	return 0;
}

void min_heap_shift_up_unconditional_(min_heap_t* s, unsigned hole_index, heap_item_t* e)
{
    unsigned parent = (hole_index - 1) / 2;
    
    do {
		(s->p[hole_index] = s->p[parent])->min_heap_idx = hole_index;
		hole_index = parent;
		parent = (hole_index - 1) / 2;
    } while (hole_index && min_heap_elem_greater(s->p[parent], e));
    
    (s->p[hole_index] = e)->min_heap_idx = hole_index;
}

void min_heap_shift_up_(min_heap_t* s, unsigned hole_index, heap_item_t* e)
{
#ifdef HEAP_DEBUG
	int dbg_cnt = 0;
#endif
    unsigned parent = (hole_index - 1) / 2;
    while (hole_index && min_heap_elem_greater(s->p[parent], e)) {
		(s->p[hole_index] = s->p[parent])->min_heap_idx = hole_index;
		hole_index = parent;
		parent = (hole_index - 1) / 2;
	#ifdef HEAP_DEBUG
		dbg_cnt++;
	#endif		
    }
#ifdef HEAP_DEBUG
	printf("%s: cnt = %d, %d\n", __FUNCTION__, dbg_cnt, hole_index);
#endif    
    (s->p[hole_index] = e)->min_heap_idx = hole_index;
}

void min_heap_shift_down_(min_heap_t* s, unsigned hole_index, heap_item_t* e)
{
#ifdef HEAP_DEBUG
	int dbg_cnt = 0;
#endif	
    unsigned min_child = 2 * (hole_index + 1);
    while (min_child <= s->n) {
		min_child -= min_child == s->n || min_heap_elem_greater(s->p[min_child], s->p[min_child - 1]);	//get min`s child index
		if (!(min_heap_elem_greater(e, s->p[min_child]))) {
			break;
		}
	#ifdef HEAP_DEBUG
		dbg_cnt++;
	#endif				
		(s->p[hole_index] = s->p[min_child])->min_heap_idx = hole_index;
		hole_index = min_child;
		min_child = 2 * (hole_index + 1);
	}
#ifdef HEAP_DEBUG
	printf("%s: cnt = %d, %d, %d\n", __FUNCTION__, dbg_cnt, hole_index, e->value);
#endif    	
    (s->p[hole_index] = e)->min_heap_idx = hole_index;
}

void main()
{
	heap_item_t in_array[HEAP_SIZE] = {{20, -1}, {12, -1}, {40, -1}, {36, -1}, {25, -1}, {45, -1}, {48, -1}, {50, -1}};
	min_heap_t min_heap;
	heap_item_t *pout;
	int i,j;
	
	min_heap_ctor_(&min_heap);
	
	for (i=0; i<HEAP_SIZE; i++) {
		min_heap_push_(&min_heap, &in_array[i]);
		printf("Push %d:\n", in_array[i].value);
		for (j=0; j<min_heap.n; j++) {
			printf("(%d,%d),", min_heap.p[j]->value, min_heap.p[j]->min_heap_idx);
		}
		printf("\n\n");
	}
	
	printf("\n");
			
	for (i=0; i<HEAP_SIZE; i++) {
		pout = min_heap_pop_(&min_heap);
		printf("Pop %d:\n", pout->value);
		for (j=0; j<min_heap.n; j++) {
			printf("(%d,%d),", min_heap.p[j]->value, min_heap.p[j]->min_heap_idx);
		}		
		printf("\n\n");
	}	
	printf("\n");
	
	min_heap_dtor_(&min_heap);
} 
