/**
 *	@filename :
 * 		test_reference.c 
 * 
 * 	@note :
 * 		
 1). 由于返回值直接指向了一个生命期尚未结束的变量，因此，对于函数返回值（或者称为函数结果）本身的任何操作，都在实际上，是对那个变量的操作，这就是引入const类型的返回的意义。
 当使用了const关键字后，即意味着函数的返回值不能立即得到修改！
 如下代码，将无法编译通过，这就是因为返回值立即进行了++操作（相当于对变量z进行了++操作），而这对于该函数而言，是不允许的。
 如果去掉const，再行编译，则可以获得通过，并且打印形成z = 7的结果。
 2). 什么时候返回引用是正确的？而什么时候返回const引用是正确的？ 
    返回指向函数调用前就已经存在的对象的引用是正确的。当不希望返回的对象被修改时，返回const引用是正确的。
 **/
#include <iostream>
#include <cstdlib>

using namespace std;

//const int& abc(int a, int b, int c, int& result){
int& abc(int a, int b, int c, int& result){
	result = a + b + c;
	return result;
}


int main() {
	int a = 1; int b = 2; int c=3;
	int z;

	abc(a, b, c, z)++;  //wrong: returning a const reference
	cout << "z= " << z << endl;
	
	return 0;
}
