/**
 * 	@filename :
 * 		test_coroutine.c
 * 
 * 	@note :
 * 		当state == 0时， state = __LINE__; return x; 下一次进来的时候， 直接跳到case __LINE__:执行， 接着执行i++,　然后执行
 * 		state = __LINE__; return x;。 
 **/ 
/* 
反汇编:
0804841d <function>:
 804841d:	55                   	push   %ebp
 804841e:	89 e5                	mov    %esp,%ebp
 8048420:	a1 24 a0 04 08       	mov    0x804a024,%eax		// %eax = state;
 8048425:	85 c0                	test   %eax,%eax			// case 0:
 8048427:	74 07                	je     8048430 <function+0x13>
 8048429:	83 f8 13             	cmp    $0x13,%eax			// case __LINE__
 804842c:	74 1f                	je     804844d <function+0x30>
 804842e:	eb 34                	jmp    8048464 <function+0x47> // return x
 8048430:	c7 05 28 a0 04 08 00 	movl   $0x0,0x804a028		// i = 0
 8048437:	00 00 00 
 804843a:	eb 1e                	jmp    804845a <function+0x3d>
 804843c:	c7 05 24 a0 04 08 13 	movl   $0x13,0x804a024		// state = __LINE__
 8048443:	00 00 00 
 8048446:	a1 28 a0 04 08       	mov    0x804a028,%eax		// %eax = i;
 804844b:	eb 17                	jmp    8048464 <function+0x47> // return x
 804844d:	a1 28 a0 04 08       	mov    0x804a028,%eax		// %eax = i;
 8048452:	83 c0 01             	add    $0x1,%eax			// +1
 8048455:	a3 28 a0 04 08       	mov    %eax,0x804a028		// i = %eax
 804845a:	a1 28 a0 04 08       	mov    0x804a028,%eax		// %eax = i;
 804845f:	83 f8 09             	cmp    $0x9,%eax			// i < 10
 8048462:	7e d8                	jle    804843c <function+0x1f>
 8048464:	5d                   	pop    %ebp
 8048465:	c3                   	ret 
*/
#include <stdio.h>


#define Begin() static int state=0; switch(state) { case 0:
#define Yield(x) do { state=__LINE__; return x; case __LINE__:; } while (0)
#define End() }

int function(void) {
	static int i;
	Begin();
	for (i = 0; i < 10; i++)
	Yield(i);
	End();
}

int main(int argc, char *argv[])
{
	int index;
	
	for (index=0; index<10; index++) {
		printf("value = %d\n", function());
	}
	
	return 0;
}
