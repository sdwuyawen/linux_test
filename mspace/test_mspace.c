/**
 * @filename :
 * 		test_mspace.c
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
#define HAVE_MMAP 0
#define HAVE_MREMAP 0
#define HAVE_MORECORE 0
#define MALLOC_ALIGNMENT 8		/* 16 */
#define MSPACES 1
#define NO_MALLINFO 1			
#define ONLY_MSPACES 1			/* Not include dl* functions, such as dlmalloc, dlfree... */
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

static void* gExecutableStore = NULL;
static mspace gMspace = NULL;
const size_t kMaxCodeCacheCapacity = 10 * 1024 * 1024;

int main(int argc, char *argv[])
{
	size_t size1 = 255;
	void *addr1 = NULL;
	size_t size2 = 256;
	void *addr2 = NULL;
	size_t size3 = kMaxCodeCacheCapacity - 512;
	void *addr3 = NULL;
		
	gExecutableStore = malloc(kMaxCodeCacheCapacity);
	if (gExecutableStore == NULL) {
		printf("error malloc\n");
		return -1;
	}
    gMspace = create_mspace_with_base(gExecutableStore, kMaxCodeCacheCapacity,
              /*locked=*/ true);
    
    printf("-> create mspace\n");
    mspace_malloc_stats(gMspace);
              
	addr1 = mspace_malloc(gMspace, size1);
	if (addr1 != NULL) {
		printf("-> malloc addr1 = %p, 0x%x\n", addr1, (uint32_t)size1);
		mspace_malloc_stats(gMspace);
		printf("addr1 size = 0x%x\n", (uint32_t)mspace_usable_size(addr1));
	}					
	
	addr2 = mspace_malloc(gMspace, size2);
	if (addr2 != NULL) {
		printf("-> malloc addr2 = %p, 0x%x\n", addr2, (uint32_t)size2);
		mspace_malloc_stats(gMspace);
		printf("addr2 size = 0x%x\n", (uint32_t)mspace_usable_size(addr2));
	}	
				
	addr3 = mspace_malloc(gMspace, size3);
	if (addr3 != NULL) {
		printf("-> malloc addr3 = %p, 0x%x\n", addr3, (uint32_t)size3);
		mspace_malloc_stats(gMspace);
		printf("addr3 size = 0x%x\n", (uint32_t)mspace_usable_size(addr3));
	} else {
		printf("malloc addr3 error!\n");
	}			

	if (addr1 != NULL) {
		mspace_free(gMspace, addr1);
	}
	
	if (addr2 != NULL) {
		mspace_free(gMspace, addr2);
	}				

	if (addr3 != NULL) {
		mspace_free(gMspace, addr3);
	}				
	
	printf("-> all free\n");
	mspace_malloc_stats(gMspace);
              
_exit:
	if (gMspace != NULL) {
		destroy_mspace(gMspace);
		gMspace = NULL;
	}
	
	if (gExecutableStore != NULL) {
		free(gExecutableStore);
		gExecutableStore = NULL;
	}			
	
	return 0;
} 
