/**
 *	@note :	 
 *
 **/
#if 0
1). 边界标记(chunk)
	struct malloc_chunk {
	  size_t               prev_foot;  /* Size of previous chunk (if free).  */
	  size_t               head;       /* Size and inuse bits. */
	  struct malloc_chunk* fd;         /* double links -- used only if free. */
	  struct malloc_chunk* bk;
	};
	
	输出:
		ptr = 0x886f008, 0x991
		next_ptr = 0x886f998, 0x6089
		after memset(0x11) 0x6089, 0x11111111
		after memset(0x22) 0x6089, 0x22222222
		after free(ptr) 0x6088, 0x990
		first chunk addr 0x886f000
		first chunk = 0x0, 0x991
	first chunk head = 0x991， 其中bit[0] = 1, 表示前一个chunk已分配, 当前chunk size = 0x990 (包括malloc_chunk中的8个字节)
	second chunk head = 0x6089,  其中bit[0] = 1, 表示前一个chunk已分配, 当前chunk size = 0x6088
	从0x11111111和0x22222222看出，前一个chunk的user data是覆盖了second chunk的prev_foot, 因为prev_foot只在
	前一个chunk是未分配时才有用到, 所以当我释放ptr之后，second chunk的prev_foot的值变为0x990.
	
	malloc_chunk结构中的fd和bk, 是在当前chunk是空闲的时才用到，为空闲chunk组成一个双向链表。
	  
	request2size(size)才是系统内部实际分配的大小 （32bit机器为例), 如例子中
	(0x98c + 4 + 7) & ~7 = 0x990
	(0x607e + 4 + 7) & ~7 = 0x6088

	/* pad request bytes into a usable size */
	#define pad_request(req) \
	   (((req) + CHUNK_OVERHEAD + CHUNK_ALIGN_MASK) & ~CHUNK_ALIGN_MASK)

	/* pad request, checking for minimum (but not maximum) */
	#define request2size(req) \
	  (((req) < MIN_REQUEST)? MIN_CHUNK_SIZE : pad_request(req))
		
	  CHUNK_OVERHEAD 	系统可用的size和用户申请的size的差值，最小是4
	  CHUNK_ALIGN_MASK	系统可用的size是8个字节对齐, 即 8 - 1 = 7
	  MIN_CHUNK_SIZE 	系统可用的size最小是16字节， 即sizeof(malloc_chunk)

2). 分箱管理(bin)
	bin是指某个双向链表的头节点，该链表的成员节点存放着某一特定范围size的空闲chunk。通过size, 快速定位bin index, 然后遍历其指向的链表，寻找合适的chunk来分配，
	或者将释放的chunk插入到链表中合适的地方。

3). 当next1_size大于0x20000-0x0c, head = 0x21002, 实际分配大小为0x21000, 而bit[1]=1是mmap出来的的
		next_ptr = 0x9660998, 0x6089
		next1_ptr = 0xf77b0008, 0x21002
		malloc_stats输出信息：
		Arena 0:
		system bytes     =     135168
		in use bytes     =      24712
		Total (incl. mmap):
		system bytes     =     270336
		in use bytes     =     159880
		max mmap regions =          1
		max mmap bytes   =     135168
    
    等于0x20000-0x0c, head = 0x1fff9, 实际分配大小为0x1fff8, 而bit[0]=1
		next_ptr = 0x8fc6998, 0x6089
		next1_ptr = 0x8fcca20, 0x1fff9    
		malloc_stats输出信息：
		Arena 0:
		system bytes     =     290816
		in use bytes     =     158224
		Total (incl. mmap):
		system bytes     =     290816
		in use bytes     =     158224
		max mmap regions =          0
		max mmap bytes   =          0
    
    (1). 当next1_size=0x10000, malloc 10000次， 只是system bytes和in use bytes改变，mmap部分没有变化。
		Arena 0:
		system bytes     =  655601664
		in use bytes     =  655467160
		Total (incl. mmap):
		system bytes     =  655601664
		in use bytes     =  655467160
		max mmap regions =          0
		max mmap bytes   =          0
    
    (2). 当next1_size=0x20000, malloc 100次， 只是system bytes和in use bytes改变，mmap部分没有变化。
		Arena 0:
		system bytes     =     135168
		in use bytes     =      27160
		Total (incl. mmap):
		system bytes     =   13651968
		in use bytes     =   13543960
		max mmap regions =        100
		max mmap bytes   =   13516800    
#endif 

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define _NEXT1_MALLOC_MAX	100

int main(int argc, char *argv[])
{
	uint32_t *ptr;
	uint32_t *next_ptr;
	uint32_t *next1_ptr;
	uint32_t *next1_ptr_array[_NEXT1_MALLOC_MAX];
	uint32_t *chunk_ptr;
	int size = 0x98c;
	int next_size = 0x607e;
	int next1_size = 0x10000;
	int cnt;
		
	ptr = malloc(size);
	printf("ptr = %p, 0x%x\n", ptr, *(ptr-1));
	printf("after malloc(ptr)\n");
	malloc_stats();	
	printf("\n");
	next_ptr = malloc(next_size);
	printf("next_ptr = %p, 0x%x\n", next_ptr, *(next_ptr-1));
	for (cnt=0; cnt<_NEXT1_MALLOC_MAX; cnt++) {
		next1_ptr = malloc(next1_size);
		//printf("next1_ptr = %p, 0x%x\n", next1_ptr, *(next1_ptr-1));
		memset(next1_ptr, 0, next1_size);
		next1_ptr_array[cnt] = next1_ptr;
	}
	printf("after malloc(next_ptr)\n");
	malloc_stats();		
	printf("\n");
	memset(ptr, 0x11, size);	
	chunk_ptr = next_ptr-2;
	printf("after memset(0x11) 0x%x, 0x%x\n", *(chunk_ptr+1), *chunk_ptr);
	memset(ptr, 0x22, size);
	printf("after memset(0x22) 0x%x, 0x%x\n", *(chunk_ptr+1), *chunk_ptr);
	
	free(ptr);
	printf("\nafter free(ptr)\n");
	malloc_stats();
	printf("\n");
	printf("after free(ptr) 0x%x, 0x%x\n", *(chunk_ptr+1), *chunk_ptr);
	ptr = (uint32_t *)((char *)chunk_ptr - (*chunk_ptr & (~7)));
	printf("first chunk addr %p\n", ptr);
	printf("first chunk = 0x%x, 0x%x\n", *ptr, *(ptr+1));
	free(next_ptr);
	for (cnt=0; cnt<_NEXT1_MALLOC_MAX; cnt++) {
		next1_ptr = next1_ptr_array[cnt];
		free(next1_ptr);
	}
	printf("\nafter free(next_ptr)\n");
	malloc_stats();
	
	return 0;
}
