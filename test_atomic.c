/**
 * 	@filename :
 * 		test_atomic.c
 * 
 * 	@note :
 * 
 **/
#include <stdio.h>

typedef int pa_bool_t;

#define __GNC__
#ifndef __x86_64__
#define __x86_64__
#endif

#if defined(__GNUC__) && (defined(__amd64__) || defined(__x86_64__))
//#warn "The native atomic operations implementation for AMD64 has not been tested thoroughly. libatomic_ops is known to not work properly on AMD64 and your gcc version is too old for the gcc-builtin atomic ops support. You have three options now: test the native atomic operations implementation for AMD64, fix libatomic_ops, or upgrade your GCC."
/* Adapted from glibc */

typedef struct pa_atomic {
    volatile int value;
} pa_atomic_t;

#define PA_ATOMIC_INIT(v) { .value = (v) }

static inline int pa_atomic_load(const pa_atomic_t *a) {
    return a->value;
}

static inline void pa_atomic_store(pa_atomic_t *a, int i) {
    a->value = i;
}

static inline int pa_atomic_add(pa_atomic_t *a, int i) {
    int result;

    __asm __volatile ("lock; xaddl %0, %1"
                      : "=r" (result), "=m" (a->value)
                      : "0" (i), "m" (a->value));

    return result;
}

static inline int pa_atomic_sub(pa_atomic_t *a, int i) {
    return pa_atomic_add(a, -i);
}

static inline int pa_atomic_inc(pa_atomic_t *a) {
    return pa_atomic_add(a, 1);
}

static inline int pa_atomic_dec(pa_atomic_t *a) {
    return pa_atomic_sub(a, 1);
}

static inline pa_bool_t pa_atomic_cmpxchg(pa_atomic_t *a, int old_i, int new_i) {
    int result;

    __asm__ __volatile__ ("lock; cmpxchgl %2, %1"
                          : "=a" (result), "=m" (a->value)
                          : "r" (new_i), "m" (a->value), "0" (old_i));

    return result == old_i;
}

typedef struct pa_atomic_ptr {
    volatile unsigned long value;
} pa_atomic_ptr_t;

#define PA_ATOMIC_PTR_INIT(v) { .value = (long) (v) }

static inline void* pa_atomic_ptr_load(const pa_atomic_ptr_t *a) {
    return (void*) a->value;
}

static inline void pa_atomic_ptr_store(pa_atomic_ptr_t *a, void *p) {
    a->value = (unsigned long) p;
}

static inline pa_bool_t pa_atomic_ptr_cmpxchg(pa_atomic_ptr_t *a, void *old_p, void* new_p) {
    void *result;

    __asm__ __volatile__ ("lock; cmpxchgq %q2, %1"
                          : "=a" (result), "=m" (a->value)
                          : "r" (new_p), "m" (a->value), "0" (old_p));

    return result == old_p;
}

#elif defined(ATOMIC_ARM_INLINE_ASM)

/*
   These should only be enabled if we have ARMv6 or better.
*/

typedef struct pa_atomic {
    volatile int value;
} pa_atomic_t;

#define PA_ATOMIC_INIT(v) { .value = (v) }

static inline void pa_memory_barrier(void) {
#ifdef ATOMIC_ARM_MEMORY_BARRIER_ENABLED
    asm volatile ("mcr  p15, 0, r0, c7, c10, 5  @ dmb");
#endif
}

static inline int pa_atomic_load(const pa_atomic_t *a) {
    pa_memory_barrier();
    return a->value;
}

static inline void pa_atomic_store(pa_atomic_t *a, int i) {
    a->value = i;
    pa_memory_barrier();
}

/* Returns the previously set value */
static inline int pa_atomic_add(pa_atomic_t *a, int i) {
    unsigned long not_exclusive;
    int new_val, old_val;

    pa_memory_barrier();
    do {
        asm volatile ("ldrex    %0, [%3]\n"
                      "add      %2, %0, %4\n"
                      "strex    %1, %2, [%3]\n"
                      : "=&r" (old_val), "=&r" (not_exclusive), "=&r" (new_val)
                      : "r" (&a->value), "Ir" (i)
                      : "cc");
    } while(not_exclusive);
    pa_memory_barrier();

    return old_val;
}

/* Returns the previously set value */
static inline int pa_atomic_sub(pa_atomic_t *a, int i) {
    unsigned long not_exclusive;
    int new_val, old_val;

    pa_memory_barrier();
    do {
        asm volatile ("ldrex    %0, [%3]\n"
                      "sub      %2, %0, %4\n"
                      "strex    %1, %2, [%3]\n"
                      : "=&r" (old_val), "=&r" (not_exclusive), "=&r" (new_val)
                      : "r" (&a->value), "Ir" (i)
                      : "cc");
    } while(not_exclusive);
    pa_memory_barrier();

    return old_val;
}

static inline int pa_atomic_inc(pa_atomic_t *a) {
    return pa_atomic_add(a, 1);
}

static inline int pa_atomic_dec(pa_atomic_t *a) {
    return pa_atomic_sub(a, 1);
}

static inline pa_bool_t pa_atomic_cmpxchg(pa_atomic_t *a, int old_i, int new_i) {
    unsigned long not_equal, not_exclusive;

    pa_memory_barrier();
    do {
        asm volatile ("ldrex    %0, [%2]\n"
                      "subs     %0, %0, %3\n"
                      "mov      %1, %0\n"
                      "strexeq %0, %4, [%2]\n"
                      : "=&r" (not_exclusive), "=&r" (not_equal)
                      : "r" (&a->value), "Ir" (old_i), "r" (new_i)
                      : "cc");
    } while(not_exclusive && !not_equal);
    pa_memory_barrier();

    return !not_equal;
}

typedef struct pa_atomic_ptr {
    volatile unsigned long value;
} pa_atomic_ptr_t;

#define PA_ATOMIC_PTR_INIT(v) { .value = (long) (v) }

static inline void* pa_atomic_ptr_load(const pa_atomic_ptr_t *a) {
    pa_memory_barrier();
    return (void*) a->value;
}

static inline void pa_atomic_ptr_store(pa_atomic_ptr_t *a, void *p) {
    a->value = (unsigned long) p;
    pa_memory_barrier();
}

static inline pa_bool_t pa_atomic_ptr_cmpxchg(pa_atomic_ptr_t *a, void *old_p, void* new_p) {
    unsigned long not_equal, not_exclusive;

    pa_memory_barrier();
    do {
        asm volatile ("ldrex    %0, [%2]\n"
                      "subs     %0, %0, %3\n"
                      "mov      %1, %0\n"
                      "strexeq %0, %4, [%2]\n"
                      : "=&r" (not_exclusive), "=&r" (not_equal)
                      : "r" (&a->value), "Ir" (old_p), "r" (new_p)
                      : "cc");
    } while(not_exclusive && !not_equal);
    pa_memory_barrier();

    return !not_equal;
}

#elif defined(ATOMIC_ARM_LINUX_HELPERS)

/* See file arch/arm/kernel/entry-armv.S in your kernel sources for more
   information about these functions. The arm kernel helper functions first
   appeared in 2.6.16.
   Apply --disable-atomic-arm-linux-helpers flag to configure if you prefer
   inline asm implementation or you have an obsolete Linux kernel.
*/
/* Memory barrier */
typedef void (__kernel_dmb_t)(void);
#define __kernel_dmb (*(__kernel_dmb_t *)0xffff0fa0)

static inline void pa_memory_barrier(void) {
#ifndef ATOMIC_ARM_MEMORY_BARRIER_ENABLED
    __kernel_dmb();
#endif
}

/* Atomic exchange (__kernel_cmpxchg_t contains memory barriers if needed) */
typedef int (__kernel_cmpxchg_t)(int oldval, int newval, volatile int *ptr);
#define __kernel_cmpxchg (*(__kernel_cmpxchg_t *)0xffff0fc0)

/* This is just to get rid of all warnings */
typedef int (__kernel_cmpxchg_u_t)(unsigned long oldval, unsigned long newval, volatile unsigned long *ptr);
#define __kernel_cmpxchg_u (*(__kernel_cmpxchg_u_t *)0xffff0fc0)

typedef struct pa_atomic {
    volatile int value;
} pa_atomic_t;

#define PA_ATOMIC_INIT(v) { .value = (v) }

static inline int pa_atomic_load(const pa_atomic_t *a) {
    pa_memory_barrier();
    return a->value;
}

static inline void pa_atomic_store(pa_atomic_t *a, int i) {
    a->value = i;
    pa_memory_barrier();
}

/* Returns the previously set value */
static inline int pa_atomic_add(pa_atomic_t *a, int i) {
    int old_val;
    do {
        old_val = a->value;
    } while(__kernel_cmpxchg(old_val, old_val + i, &a->value));
    return old_val;
}

/* Returns the previously set value */
static inline int pa_atomic_sub(pa_atomic_t *a, int i) {
    int old_val;
    do {
        old_val = a->value;
    } while(__kernel_cmpxchg(old_val, old_val - i, &a->value));
    return old_val;
}

/* Returns the previously set value */
static inline int pa_atomic_inc(pa_atomic_t *a) {
    return pa_atomic_add(a, 1);
}

/* Returns the previously set value */
static inline int pa_atomic_dec(pa_atomic_t *a) {
    return pa_atomic_sub(a, 1);
}

/* Returns TRUE when the operation was successful. */
static inline pa_bool_t pa_atomic_cmpxchg(pa_atomic_t *a, int old_i, int new_i) {
    pa_bool_t failed;
    do {
      failed = !!__kernel_cmpxchg(old_i, new_i, &a->value);
    } while(failed && a->value == old_i);
    return !failed;
}

typedef struct pa_atomic_ptr {
    volatile unsigned long value;
} pa_atomic_ptr_t;

#define PA_ATOMIC_PTR_INIT(v) { .value = (unsigned long) (v) }

static inline void* pa_atomic_ptr_load(const pa_atomic_ptr_t *a) {
    pa_memory_barrier();
    return (void*) a->value;
}

static inline void pa_atomic_ptr_store(pa_atomic_ptr_t *a, void *p) {
    a->value = (unsigned long) p;
    pa_memory_barrier();
}

static inline pa_bool_t pa_atomic_ptr_cmpxchg(pa_atomic_ptr_t *a, void *old_p, void* new_p) {
    pa_bool_t failed;
    do {
        failed = !!__kernel_cmpxchg_u((unsigned long) old_p, (unsigned long) new_p, &a->value);
    } while(failed && a->value == (unsigned long) old_p);
    return !failed;
}

#else

/* libatomic_ops based implementation */

#include <atomic_ops.h>

typedef struct pa_atomic {
    volatile AO_t value;
} pa_atomic_t;

#define PA_ATOMIC_INIT(v) { .value = (AO_t) (v) }

static inline int pa_atomic_load(const pa_atomic_t *a) {
    return (int) AO_load_full((AO_t*) &a->value);
}

static inline void pa_atomic_store(pa_atomic_t *a, int i) {
    AO_store_full(&a->value, (AO_t) i);
}

static inline int pa_atomic_add(pa_atomic_t *a, int i) {
    return (int) AO_fetch_and_add_full(&a->value, (AO_t) i);
}

static inline int pa_atomic_sub(pa_atomic_t *a, int i) {
    return (int) AO_fetch_and_add_full(&a->value, (AO_t) -i);
}

static inline int pa_atomic_inc(pa_atomic_t *a) {
    return (int) AO_fetch_and_add1_full(&a->value);
}

static inline int pa_atomic_dec(pa_atomic_t *a) {
    return (int) AO_fetch_and_sub1_full(&a->value);
}

static inline pa_bool_t pa_atomic_cmpxchg(pa_atomic_t *a, int old_i, int new_i) {
    return AO_compare_and_swap_full(&a->value, (unsigned long) old_i, (unsigned long) new_i);
}

typedef struct pa_atomic_ptr {
    volatile AO_t value;
} pa_atomic_ptr_t;

#define PA_ATOMIC_PTR_INIT(v) { .value = (AO_t) (v) }

static inline void* pa_atomic_ptr_load(const pa_atomic_ptr_t *a) {
    return (void*) AO_load_full((AO_t*) &a->value);
}

static inline void pa_atomic_ptr_store(pa_atomic_ptr_t *a, void *p) {
    AO_store_full(&a->value, (AO_t) p);
}

static inline pa_bool_t pa_atomic_ptr_cmpxchg(pa_atomic_ptr_t *a, void *old_p, void* new_p) {
    return AO_compare_and_swap_full(&a->value, (AO_t) old_p, (AO_t) new_p);
}
#endif

static pa_atomic_t g_test_atomic = PA_ATOMIC_INIT(0);

int main(int argc, char *argv[]) {
	int ret;
	
	if (argc > 1) {
		int val;
		
		val = atoi(argv[1]);		
		pa_atomic_store(&g_test_atomic, val);
	}
	
	printf("init value %d\n", pa_atomic_load(&g_test_atomic));
	
	/*
	 * 如果赋值成功，返回true; 否则返回false.
	 */ 
	ret = pa_atomic_cmpxchg(&g_test_atomic, 0, 1);
	printf("[0-1]ret = %d, cnt = %d\n", ret, pa_atomic_load(&g_test_atomic));
	if (!ret) {
		ret = pa_atomic_cmpxchg(&g_test_atomic, 1, 0);
		printf("[1-0]ret = %d, cnt = %d\n", ret, pa_atomic_load(&g_test_atomic));
	}	
	
	return 0;
}
