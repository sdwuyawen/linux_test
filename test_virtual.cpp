#include <stdio.h>

class A
{
public:
	A(){};
	~A(){};
	void f1(){printf("A f1\n");};
	virtual void f2(){printf("A f2");};			//must define function entity
	virtual void f3()=0;						//subclass define function entity
};

class B:public A
{
public:
	B(){};
	~B(){};
	void f1(){printf("B f1\n");};
	virtual void f2(){printf("B f2\n");};
	virtual void f3(){printf("B f3\n");};
};

int main(int argc, char* argv[])
{
	A* m_a = new B();
	B* m_b = new B();

	m_a->f1();		//call A->f1
	m_a->f2();		//call B->f2, 指向对象的函数
	m_a->f3();		//call B->f3
	
	m_b->f1();		//call B->f1

	delete m_a;
	delete m_b;
	
	return 0;
}
