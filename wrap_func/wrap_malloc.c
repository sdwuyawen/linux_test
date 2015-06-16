#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

/*
 */
#define wrap(a) __wrap_ ## a

#define MAX_SIZE_T           (~(size_t)0)
#define SIZE_T_SIZE			sizeof(size_t)
#define MEM_SIZE_T_OFFSET	(2)
#define MEM_OFFSET			(SIZE_T_SIZE*MEM_SIZE_T_OFFSET)

static size_t g_mem_used_size = 0;
static int g_mem_debug = 0;

static inline size_t _get_call_addr(void)
{
	volatile int *p;
#if defined(__arm__)	
    __asm__(
        "mov %0, lr\n"
        : "=r"(p)
    );
	return (size_t)p - SIZE_T_SIZE;
#elif defined(__mips__)	
    __asm__(
        "move %0, $ra\n"
        : "=r"(p)
    );
    return (size_t)p - MEM_OFFSET;
#elif defined(__x86_64__)
    __asm__ volatile(
        "mov 8(%%rbp), %0\n"
        : "=r"(p)
    );
    return (size_t)p;
#else	//__i386__
    __asm__ volatile(
        "mov 4(%%ebp), %0\n"
        : "=r"(p)
    );
    return (size_t)p;
#endif    
}
 
/*
 */
void *__real_malloc(size_t size);
void *wrap(malloc)(size_t size)
{	
	size_t *ptr;	
	    
	volatile size_t addr = _get_call_addr();

	size += MEM_OFFSET;
	ptr = (size_t *)__real_malloc(size);
	if (ptr != NULL) {
		if (g_mem_debug) {
			printf("%s 0x%"__PRIPTR_PREFIX"x\n", __func__, addr);
		}		
		__sync_fetch_and_add(&g_mem_used_size, size);
		*ptr = size;	
		*(ptr + 1) = addr;	
		ptr += MEM_SIZE_T_OFFSET;
	}
	
	return (void *)ptr;
}

void __real_free(void *ptr);
void wrap(free)(void *ptr)
{
	size_t size;
	size_t *p;
	
	if (ptr == NULL) {
		return;
	}
	
	p = (size_t *)ptr;
	p -= MEM_SIZE_T_OFFSET;
	size = *p;
	if (g_mem_debug) {
		printf("%s 0x%"__PRIPTR_PREFIX"x\n", __func__, *(p+1));
	}	
	__sync_fetch_and_sub(&g_mem_used_size, size);
	
	__real_free((void *)p);
}

void *wrap(realloc)(void *ptr, size_t size)
{
	void *new_ptr = NULL;
	size_t old_size;
	
	if (size == 0) {
		__wrap_free(ptr);
		return NULL;
	} 
	
	volatile size_t addr = _get_call_addr();
		
	size_t req_size = size + MEM_OFFSET;
	new_ptr = __real_malloc(req_size);
	if (new_ptr != NULL) {
		if (g_mem_debug) {
			printf("%s 0x%"__PRIPTR_PREFIX"x\n", __func__, addr);
		}		
		__sync_fetch_and_add(&g_mem_used_size, req_size);
		*((size_t *)new_ptr) = req_size;	
		*((size_t *)new_ptr + 1) = addr;	
		new_ptr = (char *)new_ptr + MEM_OFFSET;		
		
		if (ptr != NULL) {
			old_size = *((size_t *)ptr - 1) - MEM_OFFSET;
			if (old_size > size) {
				old_size = size;
			}
			memcpy(new_ptr, ptr, old_size);
		}
		wrap(free)(ptr);
	}
	
	return new_ptr;	
}

void* wrap(calloc)(size_t n_elements, size_t elem_size) {
	void* mem;
	size_t req = 0;
	
	if (n_elements != 0) {
		req = n_elements * elem_size;
		if (((n_elements | elem_size) & ~(size_t)0xffff) &&
			(req / n_elements != elem_size)) {
			req = MAX_SIZE_T; /* force downstream failure on overflow */
		}
	}
	
	//printf("__wrap_calloc\n");
	volatile size_t addr = _get_call_addr();
	
	size_t req_size = req + MEM_OFFSET;
	mem = __real_malloc(req_size);
	if (mem != NULL) {		
		if (g_mem_debug) {
			printf("%s 0x%"__PRIPTR_PREFIX"x\n", __func__, addr);
		}		
		__sync_fetch_and_add(&g_mem_used_size, req_size);
		*((size_t *)mem) = req_size;	
		*((size_t *)mem + 1) = addr;	
		mem = (char *)mem + MEM_OFFSET;		

		memset(mem, 0, req);
	}
	
	return mem;	
}

#if defined(__x86_64__)
void *wrap(_Znwm)(size_t size)
#else
void *wrap(_Znwj)(size_t size)
#endif
{
	size_t *ptr;	
	    
	volatile size_t addr = _get_call_addr();

	size += MEM_OFFSET;
	ptr = (size_t *)__real_malloc(size);
	if (ptr != NULL) {
		if (g_mem_debug) {
			printf("%s 0x%"__PRIPTR_PREFIX"x\n", __func__, addr);    
		}		
		__sync_fetch_and_add(&g_mem_used_size, size);
		*ptr = size;	
		*(ptr + 1) = addr;	
		ptr += MEM_SIZE_T_OFFSET;
	}
	
	return (void *)ptr;
}

void wrap(_ZdlPv)(void *ptr)
{	
	size_t size;
	size_t *p;
	
	if (ptr == NULL) {
		return;
	}
	
	p = (size_t *)ptr;
	p -= MEM_SIZE_T_OFFSET;
	size = *p;
	if (g_mem_debug) {
		printf("%s 0x%"__PRIPTR_PREFIX"x\n", __func__, *(p+1));
	}	
	__sync_fetch_and_sub(&g_mem_used_size, size);
	
	__real_free((void *)p);
}

#if defined(__x86_64__)
void *wrap(_Znam)(size_t size)
#else
void *wrap(_Znaj)(size_t size)
#endif
{
	size_t *ptr;	
	    
	volatile size_t addr = _get_call_addr();

	size += MEM_OFFSET;
	ptr = (size_t *)__real_malloc(size);
	if (ptr != NULL) {
		if (g_mem_debug) {
			printf("%s 0x%"__PRIPTR_PREFIX"x\n", __func__, addr);
		}		
		__sync_fetch_and_add(&g_mem_used_size, size);
		*ptr = size;	
		*(ptr + 1) = addr;	
		ptr += MEM_SIZE_T_OFFSET;
	}
	
	return (void *)ptr;
}

void wrap(_ZdaPv)(void *ptr)
{
	size_t size;
	size_t *p;
	
	if (ptr == NULL) {
		return;
	}
	
	p = (size_t *)ptr;
	p -= MEM_SIZE_T_OFFSET;
	size = *p;
	if (g_mem_debug) {
		printf("%s 0x%"__PRIPTR_PREFIX"x\n", __func__, *(p+1));
	}	
	__sync_fetch_and_sub(&g_mem_used_size, size);
	
	__real_free((void *)p);
}

char *wrap(strndup)(const char *src, size_t len)
{
	char* dst = (char*) wrap(malloc)(len + 1);
	if (dst) {
		strncpy(dst, src, len + 1);
	}
	
	return dst; 
}

char *wrap(__strdup)(const char *src)
{
	size_t len = strlen(src);
	
	return wrap(strndup)(src, len); 
}

char *wrap(strdup)(const char *src)
{
	size_t len = strlen(src);
	
	return wrap(strndup)(src, len); 
}

static __attribute__((destructor)) void wrap_malloc_exit( void );
static __attribute__((constructor)) void wrap_malloc_enter( void );

static void wrap_malloc_enter(void)
{
	printf("--> %s\n", __func__);
}

static void wrap_malloc_exit(void) 
{
	printf("--> %s : memory used size = 0x%"__PRIPTR_PREFIX"x\n", __func__, g_mem_used_size);
}

void wrap_malloc_info(void) 
{
	printf("memory used size = 0x%"__PRIPTR_PREFIX"x\n", g_mem_used_size);
}

void wrap_malloc_debug(int enable) 
{
	g_mem_debug = enable;
}
