/**
 *	@filename :
 * 		test_++.c
 * 	
 * 	@note :
 * 		(x86_64和arm平台测试结果)
 *		输出结果是r=60, i=3
 * 		
 * 		参照标准的5.1.2.3章节，那里定义了顺序点的细节。但在我们的例子中，我们的表达式展示了未定义行为。它完全取决于编译器对于 
 * 		什么时候 给变量分配新值的副作用会执行 相对于表达式的其他部分。
 *		最好的应用是避免构造复杂的前缀后缀表达式
 **/
/*
	带注释的x86_64反汇编代码。
	Disassembly of section .text:
	 
	0000000000000000 <main>:
	#include <stdio.h>
	 
	int main(){
	0: 55 push %rbp
	1: 48 89 e5 mov %rsp,%rbp
	4: 48 83 ec 20 sub $0x20,%rsp
	int i = 0;
	8: c7 45 e8 00 00 00 00 movl $0x0,-0x18(%rbp)
	int a[] = {10,20,30};
	f: c7 45 f0 0a 00 00 00 movl $0xa,-0x10(%rbp)
	16: c7 45 f4 14 00 00 00 movl $0x14,-0xc(%rbp)
	1d: c7 45 f8 1e 00 00 00 movl $0x1e,-0x8(%rbp)
	int r = 1 * a[i++] + 2 * a[i++] + 3 * a[i++];
	24: 8b 45 e8 mov -0x18(%rbp),%eax
	27: 48 98 cltq
	29: 8b 54 85 f0 mov -0x10(%rbp,%rax,4),%edx
	2d: 8b 45 e8 mov -0x18(%rbp),%eax
	30: 48 98 cltq
	32: 8b 44 85 f0 mov -0x10(%rbp,%rax,4),%eax
	36: 01 c0 add %eax,%eax
	38: 8d 0c 02 lea (%rdx,%rax,1),%ecx
	3b: 8b 45 e8 mov -0x18(%rbp),%eax
	3e: 48 98 cltq
	40: 8b 54 85 f0 mov -0x10(%rbp,%rax,4),%edx
	44: 89 d0 mov %edx,%eax
	46: 01 c0 add %eax,%eax
	48: 01 d0 add %edx,%eax
	4a: 01 c8 add %ecx,%eax
	4c: 89 45 ec mov %eax,-0x14(%rbp)
	4f: 83 45 e8 01 addl $0x1,-0x18(%rbp)
	53: 83 45 e8 01 addl $0x1,-0x18(%rbp)
	57: 83 45 e8 01 addl $0x1,-0x18(%rbp)
	printf("%d\n", r);
	5b: 8b 45 ec mov -0x14(%rbp),%eax
	5e: 89 c6 mov %eax,%esi
	60: bf 00 00 00 00 mov $0x0,%edi
	65: b8 00 00 00 00 mov $0x0,%eax
	6a: e8 00 00 00 00 callq 6f <main+0x6f>
	return 0;
	6f: b8 00 00 00 00 mov $0x0,%eax
	}
	74: c9 leaveq
	75: c3 retq
*/
#include <stdio.h>
 
int main() 
{
	int i = 0;
	int a[] = {10,20,30};
	 
	int r = 1 * a[i++] + 2 * a[i++] + 3 * a[i++];
	
	printf("r=%d, i=%d\n", r, i);
	
	return 0;
}
