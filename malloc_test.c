/**
	@filename :
		malloc_test.c
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

/**
**/
#define EXIT_FAIL_NONSTARTER    0x01
#define EXIT_FAIL_ADDRESSLINES  0x02
#define EXIT_FAIL_OTHERTEST     0x04

/**
**/
#ifdef _SC_PAGE_SIZE
int memtester_pagesize(void) {
    int pagesize = sysconf(_SC_PAGE_SIZE);
    if (pagesize == -1) {
        perror("get page size failed");
        exit(EXIT_FAIL_NONSTARTER);
    }
    printf("pagesize is %ld\n", (long) pagesize);
    return pagesize;
}
#else
int memtester_pagesize(void) {
    printf("sysconf(_SC_PAGE_SIZE) not supported; using pagesize of 8192\n");
    return 8192;
}
#endif

/**
**/
int main(int argc, char *argv[]) {
		
    int fill = 1;
    size_t total = 0;
    size_t size = 0x40000000;
	size_t pagesize, bufsize;
    ptrdiff_t pagesizemask;
    void volatile *buf, *aligned;

    pagesize = memtester_pagesize();
    pagesizemask = (ptrdiff_t) ~(pagesize - 1);
    printf("pagesizemask is 0x%tx\n", pagesizemask);

    while (size) {
        void* addr = malloc(size);
        if (addr == 0) {
            printf("size = %9zd failed\n", size);
            size >>= 1;
        } else {
		#if 0
			bufsize = size;
			buf = (void *)addr;
            if ((size_t) buf % pagesize) {
                /* printf("aligning to page -- was 0x%tx\n", buf); */
                aligned = (void volatile *) ((size_t) buf & pagesizemask) + pagesize;
                /* printf("  now 0x%tx -- lost %d bytes\n", aligned,
                 *      (size_t) aligned - (size_t) buf);
                 */
                bufsize -= ((size_t) aligned - (size_t) buf);
            } else {
                aligned = buf;
            }
            /* Try mlock */
            if (mlock((void *) aligned, bufsize) < 0) {
				free((void *)buf);
				size >>= 1;
				continue;
			}
		#endif
            total += size;
            printf("size = %9zd, addr = %p (total = %9zd (%zd MB))\n",
                    size, addr, total, total / (1024*1024));
            if (fill) {
                printf("filling...\n");
                fflush(stdout);
                memset(addr, 0, size);
            }
            size = size + (size>>1);
        }
    }

    printf("done. allocated %zd MB\n", total / (1024*1024));

	return 0;
}
