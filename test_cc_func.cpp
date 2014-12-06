/*
 * 	@filename :
 * 		test_cc_func.cpp
 * 
 * 	@note :
 * 		c++的函数指针的大小是其他的指针的２倍, １.存储函数的地址，２．存储 this 指针位置如何调整的信息（偏移）
 *	测试输出：
		------------------------------
		Object ptr:	0xffa2da5c
		Function ptr:	0x80488ca
		Pointer adj:	0
		A's this:	0xffa2da5c
		------------------------------
		Object ptr:	0xffa2da5c
		Function ptr:	0x8048906
		Pointer adj:	0x20
		B's this:	0xffa2da7c	
 * 
 */
#include <iostream>
#include <cstring>

using namespace std;
 
struct A {
    void foo() const {
        cout << "A's this:\t" << this << std::endl;
    }
    char pad0[32];
};
 
struct B {
    void bar() const {
        cout << "B's this:\t" << this << std::endl;
    }
    char pad2[64];
};
 
struct C : A, B
{ };
 
void call_by_ptr(const C &obj, void (C::*mem_func)() const)
{
    void *data[2];
    memcpy(data, &mem_func, sizeof(mem_func));
    cout << "------------------------------\n"
        "Object ptr:\t" << &obj <<
        "\nFunction ptr:\t" << data[0] <<
        "\nPointer adj:\t" << data[1] << std::endl;
    (obj.*mem_func)();
}
 
int main()
{
    C obj;
    call_by_ptr(obj, &C::foo);
    call_by_ptr(obj, &C::bar);
}

