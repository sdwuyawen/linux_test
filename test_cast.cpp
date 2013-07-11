/**
 * @filename :
 * 		test_cast.cpp
 * 
 * @note :
 * 
 * 
 **/
/*
1)  static_cast
    用法：static_cast <typeid> (expression)
    说明：该运算符把expression转换为typeid类型，但没有运行时类型检查来确保转换的安全性。
    用途：
    a) 用于类层次结构中基类和派生类之间指针或者引用的转换。
		up-casting (把派生类的指针或引用转换成基类的指针或者引用表示)是安全的；
		down-casting(把基类指针或引用转换成子类的指针或者引用)是不安全的。
    b) 用于基本数据类型之间的转换，如把int转换成char，这种转换的安全性也要由开发人员来保证。
    c) 可以把空指针转换成目标类型的空指针(null pointer)。
    d) 把任何类型的表达式转换成void类型。
    注意： static_cast不能转换掉expression的const、volitale或者__unaligned属性。

2)  dynamic_cast
    用法：dynamic_cast <typeid> (expression)
    说明：该运算符把expression转换成typeid类型的对象。typeid必须是类的指针、类的引用或者void*。如果typeid是类的指针类型，
        那么expression也必须是指针，如果typeid是一个引用，那么expression也必须是一个引用。一般情况下，dynamic_cast用
        于具有多态性的类(即有虚函数的类)的类型转换。
           dynamic_cast依赖于RTTI信息，其次，在转换时，dynamic_cast会检查转换的source对象是否真的可以转换成target类型，
       这种检查不是语法上的，而是真实情况的检查。先看RTTI相关部分，通常，许多编译器都是通过vtable找到对象的RTTI信息
       的，这也就意味着，如果基类没有虚方法，也就无法判断一个基类指针变量所指对象的真实类型，这时候，dynamic_cast只能
       用来做安全的转换,例如从派生类指针转换成基类指针。而这种转换其实并不需要dynamic_cast参与。也就是说,dynamic_cast
       是根据RTTI记载的信息来判断类型转换是否合法的。

    用途：主要用于类层次之间的up-casting和down-casting，还可以用于类之间的交叉转换。在进行down-casting时，dynamic_cast
        具有类型检查的功能，比static_cast更安全。检测在运行时进行。如果被转换的指针不是一个被请求的有效完整的对象指针，
        返回值为NULL。当用于多态类型时，它允许任意的隐式类型转换以及相反过程。不过，与static_cast不同，在后一种情况里
        （注：即隐式转 换的相反过程），dynamic_cast会检查操作是否有效。也就是说，它会检查转换是否会返回一个被请求的有
        效的完整对象。
    注意：dynamic_cast不能转换掉expression的const、volitale或者__unaligned属性。

3)   reinterpret_cast
     用法：reinterpret_cast <typeid>(expression)
     说明：转换一个指针为其他类型的指针，也允许将一个指针转换为整数类型，反之亦然。这个操作符能够在非相关的类型之间进行
        转换。操作结果只是简单的从一个指针到别的指针的值的二进制拷贝，在类型之间指向的内容不做任何类型的检查和转换。这
        是一个强制转换。使用时有很大的风险，慎用之。
     注意：reinterpret _cast不能转换掉expression的const、volitale或者__unaligned属性。
 
4)   const_cast
     用法：const_cast<typeid>(expression)
     说明：这个类型操纵传递对象的const属性，或者是设置或者是移除。如：
           Class C{…}
           const C* a = new C;
           C* b = const_cast<C*>(a);
           如果将上面的const_cast换成其他任何其他的转换，编译都不能通过，出错的信息大致如下：
           “…cannot convert from 'const class C *' to 'class C *'”。

*/ 
#include <iostream>

using namespace std;
 
class Base {
public:
	int _base;
	virtual void printinfo() {
		cout << _base << endl;
	}
};

class Derived : public Base {
public:
	int _derived;
	virtual void printinfo() {
		cout << _derived << endl;
	}
};

int main(void) {
	Base b1;
	Derived d1;
	int aInt = 10;
	long aLong = 11;
	float aFloat = 11.11f;
	double aDouble = 12.12;
	
	Derived* pd = static_cast<Derived*>(&b1);                  	// down-casting          不安全
	Base* pb = static_cast<Base*>(&d1);                         	// up-casting            安全
	Derived& d = static_cast<Derived&>(b1);                    	// down-casting          不安全
	Base& b = static_cast<Base&>(d1);                           	// up-casting            安全
	
	
	aInt = static_cast<int>(aFloat);                             	// 基本数据类型转换
	void* sth = static_cast<void*>(&aDouble);                   	// 将double指针类型转换成void指针类型
	double* bDouble = static_cast<double*>(sth);               	// 将void指针类型转换成double指针类型
	cout << *bDouble << endl;
			
	Base* pb1 = dynamic_cast<Base*>(&d1);
	//Derived* pd1 = dynamic_cast<Derived*>(&b1);                 	// 编译时有warning，运行时出错
	
	int bInt = reinterpret_cast<int>(pb1);                     	// 将地址或指针转换成整数
	cout << bInt << endl;
	pb1 = reinterpret_cast<Base*>(bInt);                       	// 将整数转换成地址或指针
	
	int* cInt = reinterpret_cast<int*>(&aFloat);               	// 这个转换的结果会出乎意料
	cout << (int)*cInt << endl;
	
	const Base* bBase = new Base();
	Base* cBase = const_cast<Base*>(bBase);
	//Base* dBase = dynamic_cast<Base*>(bBase);                		// 不能通过编译
	//Base* eBase = static_cast<Base*>(bBase);                     	// 不能通过编译
	//Base* fBase = reinterpret_cast<Base*>(bBase);             	// 不能通过编译
	
	return 0;
}
