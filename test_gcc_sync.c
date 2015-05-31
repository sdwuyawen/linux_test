/**
 * @filename :
 * 	test_gcc_sync.c
 * 
 * @note :
gcc从4.1.2提供了__sync_*系列的built-in函数，用于提供加减和逻辑运算的原子操作:
其声明如下：
type __sync_fetch_and_add (type *ptr, type value, ...)
type __sync_fetch_and_sub (type *ptr, type value, ...)
type __sync_fetch_and_or (type *ptr, type value, ...)
type __sync_fetch_and_and (type *ptr, type value, ...)
type __sync_fetch_and_xor (type *ptr, type value, ...)
type __sync_fetch_and_nand (type *ptr, type value, ...)


type __sync_add_and_fetch (type *ptr, type value, ...)
type __sync_sub_and_fetch (type *ptr, type value, ...)
type __sync_or_and_fetch (type *ptr, type value, ...)
type __sync_and_and_fetch (type *ptr, type value, ...)
type __sync_xor_and_fetch (type *ptr, type value, ...)
type __sync_nand_and_fetch (type *ptr, type value, ...)

这两组函数的区别在于第一组返回更新前的值，第二组返回更新后的值。

后面的可扩展参数(...)用来指出哪些变量需要memory barrier,因为目前gcc实现的是full barrier（类似于linux kernel 中的mb(),
表示这个操作之前的所有内存操作不会被重排序到这个操作之后）,所以可以略掉这个参数。
bool __sync_bool_compare_and_swap (type *ptr, type oldval type newval, ...)
type __sync_val_compare_and_swap (type *ptr, type oldval type newval, ...)


这两个函数提供原子的比较和交换，如果*ptr == oldval,就将newval写入*ptr,
第一个函数在相等并写入的情况下返回true.
第二个函数在返回操作之前的值。

__sync_synchronize()
发出一个full barrier.

关于memory barrier,cpu会对我们的指令进行排序，一般说来会提高程序的效率，但有时候可能造成我们不希望得到的结果，
举一个例子，比如我们有一个硬件设备，它有4个寄存器，当你发出一个操作指令的时候，一个寄存器存的是你的操作指令（比如READ），
两个寄存器存的是参数（比如是地址和size），最后一个寄存器是控制寄存器，在所有的参数都设置好之后向其发出指令，设备开始读取参数，执行命令，
程序可能如下：
    write1(dev.register_size,size);
    write1(dev.register_addr,addr);
    write1(dev.register_cmd,READ);
    write1(dev.register_control,GO);

如果最后一条write1被换到了前几条语句之前，那么肯定不是我们所期望的，这时候我们可以在最后一条语句之前加入一个memory barrier,
强制cpu执行完前面的写入以后再执行最后一条：
    write1(dev.register_size,size);
    write1(dev.register_addr,addr);
    write1(dev.register_cmd,READ);
    __sync_synchronize();
    write1(dev.register_control,GO);

memory barrier有几种类型：
    acquire barrier : 不允许将barrier之后的内存读取指令移到barrier之前（linux kernel中的wmb()）。
    release barrier : 不允许将barrier之前的内存读取指令移到barrier之后 (linux kernel中的rmb())。
    full barrier    : 以上两种barrier的合集(linux kernel中的mb())。


还有两个函数：
type __sync_lock_test_and_set (type *ptr, type value, ...)
   将*ptr设为value并返回*ptr操作之前的值。

void __sync_lock_release (type *ptr, ...)
     将*ptr置0
 * 
 **/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

static int count = 0;

void *test_func(void *arg)
{
	int i=0;
	
	for(i=0;i<20000;++i){
	#if 1
		__sync_fetch_and_add(&count,1);
	#else
		count += 1;
	#endif
	}
	
	return NULL;
}

int main(int argc, const char *argv[])
{
	pthread_t id[20];
	int i = 0;

	for(i=0;i<20;++i){
		pthread_create(&id[i],NULL,test_func,NULL);
	}

	for(i=0;i<20;++i){
		pthread_join(id[i],NULL);
	}

	printf("%d\n",count);
	
	return 0;
}
