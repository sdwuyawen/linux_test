(1).
int32_t android_atomic_inc(volatile int32_t* addr);
int32_t android_atomic_dec(volatile int32_t* addr);

(2).
int32_t android_atomic_add(int32_t value, volatile int32_t* addr);

(3).
int32_t android_atomic_and(int32_t value, volatile int32_t* addr);
int32_t android_atomic_or(int32_t value, volatile int32_t* addr);

(4). 从addr指向的地址获取内容
int32_t android_atomic_acquire_load(volatile const int32_t* addr);
int32_t android_atomic_release_load(volatile const int32_t* addr);

acquire : 在赋值语句之后调用android_memory_barrier, 保证赋值语句先执行
release : 在赋值语句之前调用android_memory_barrier, 保证前面内容先执行, 后执行赋值语句

(5). 把value存储到addr指向的地址
void android_atomic_acquire_store(int32_t value, volatile int32_t* addr);
void android_atomic_release_store(int32_t value, volatile int32_t* addr);

(6). 比较oldvalue = *addr, 把*addr = newvalue, 并返回0(假); 否则，*addr不修改，返回1(真).
int android_atomic_acquire_cas(int32_t oldvalue, int32_t newvalue,
        volatile int32_t* addr);
int android_atomic_release_cas(int32_t oldvalue, int32_t newvalue,
        volatile int32_t* addr);
        
(7).别名        
#define android_atomic_write android_atomic_release_store
#define android_atomic_cmpxchg android_atomic_release_cas        
