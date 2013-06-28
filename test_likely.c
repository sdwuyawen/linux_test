/**
 *	@filename :
 * 		test_likely.c
 * 
 * 	@note :
 * 		(1).
 * 		Built-in Function: long __builtin_expect (long EXP, long C)
 *    	You may use `__builtin_expect' to provide the compiler with branch
 *    	prediction information.  In general, you should prefer to use
 *    	actual profile feedback for this (`-fprofile-arcs'), as
 *    	programmers are notoriously bad at predicting how their programs
 *    	actually perform.  However, there are applications in which this
 *    	data is hard to collect.
 *
 *    	The return value is the value of EXP, which should be an integral
 *    	expression.  The value of C must be a compile-time constant.  The
 *    	semantics of the built-in are that it is expected that EXP == C
 * 
 * 		(2).
 * 		使用likely ，执行if后面语句的可能性大些，编译器将if{}是的内容编译到前面, 
 * 		使用unlikely ，执行else后面语句的可能性大些,编译器将else{}里的内容编译到前面。
 * 		这样有利于cpu预取,提高预取指令的正确率,因而可提高效率
 * 		(3). 前面加!!, 是返回的值只有0和1(false和true)
 * 
 **/ 
#include <stdio.h>

#ifndef likely
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif

int main(int argc, char *argv[]) {
	int i;
	
	if (argc < 2) {
		printf("%s num\n", argv[1]);
		return -1;
	}
	
	i = atoi(argv[1]);
	printf("i=%d, !i=%d, !!i=%d\n", i, !i, !!i);
	if (likely(i)) {//优先处理if的分支		
		printf("likely i=%d -> go if\n", i);
	} else {		
	}
	
	if (unlikely(!i)) {//优先处理else的分支		
	} else {
		printf("unlikely !i=%d -> go else\n", !i);
	}

	if (likely(!i)) {//优先处理if的分支		
		printf("likely if !i=%d -> go if\n", !i);
	} else {		
	}

	if (unlikely(i)) {//优先处理else的分支		
	} else {
		printf("unlikely else i=%d -> go else\n", i);
	}
	
	return 0;
}
