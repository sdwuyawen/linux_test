/**
 * @filename :
 * 		test_dlmalloc.c
 * 
 * @note :
 * 
 **/
#include <stdio.h>
#include <inttypes.h>

#ifndef false
#define false 0
#define true  1
#endif

// A dlmalloc mspace is used to manage the code cache over a mmaped region.
//#define HAVE_MMAP 0
//#define HAVE_MREMAP 0
//#define HAVE_MORECORE 0
//#define MALLOC_ALIGNMENT 8		/* 16 */
//#define MSPACES 1
//#define NO_MALLINFO 1			
//#define ONLY_MSPACES 1			/* Not include dl* functions, such as dlmalloc, dlfree... */
// Custom heap error handling.
#define PROCEED_ON_ERROR 0
static void heap_error(const char* msg, const char* function, void* p);
#define CORRUPTION_ERROR_ACTION(m) \
    heap_error("HEAP MEMORY CORRUPTION", __FUNCTION__, NULL)
#define USAGE_ERROR_ACTION(m,p) \
    heap_error("ARGUMENT IS INVALID HEAP ADDRESS", __FUNCTION__, p)
    
#define USE_LOCKS 1    

#include "malloc.c"

static void heap_error(const char* msg, const char* function, void* p) {
    fprintf(stderr, "@@@ ABORTING: CODE FLINGER: %s IN %s addr=%p",
         msg, function, p);	
    /* So that we can get a memory dump around p */
    *((int **) 0xdeadbaad) = (int *) p;
}

int main(int argc, char *argv[])
{
	size_t size1 = 255;
	void *addr1 = NULL;
	size_t size2 = 256;
	void *addr2 = NULL;
                            
	addr1 = dlmalloc(size1);
	if (addr1 != NULL) {
		printf("-> malloc addr1 = %p, 0x%x\n", addr1, (uint32_t)size1);
		dlmalloc_stats();
		printf("addr1 size = 0x%x\n", (uint32_t)dlmalloc_usable_size(addr1));
	}					
	printf("sbrk(0) = %p\n", sbrk(0));
	addr2 = dlmalloc(size2);
	if (addr2 != NULL) {
		printf("-> malloc addr2 = %p, 0x%x\n", addr2, (uint32_t)size2);
		dlmalloc_stats();
		printf("addr2 size = 0x%x\n", (uint32_t)dlmalloc_usable_size(addr2));
	}					
	
	if (addr1 != NULL) {
		dlfree(addr1);
	}
	
	if (addr2 != NULL) {
		dlfree(addr2);
	}				
	
	printf("-> all free\n");
	dlmalloc_stats();
              
_exit:	
	return 0;
} 
