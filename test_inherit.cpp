/**
   @filename :
   		test_inherit.cpp
**/
#include <stdio.h>

class ClassA
{
public:
	 ClassA();
	 ~ClassA();
	 
	 virtual void func1(void);
};

ClassA::ClassA() {
	 printf("class A\n");
}

ClassA::~ClassA() {
	 printf("~class A\n");
}

void ClassA::func1(void) {
	printf("A : %s\n", __FUNCTION__);
}

class ClassB : public ClassA
{
public:
	 ClassB(int id);
	 ~ClassB();
	 
	 void func1(void);

public:
	 int data;

private:
	 int mId;
};

ClassB::ClassB(int id) {
	 printf("class B : id=%d\n", id);
	 mId = id;
}

ClassB::~ClassB(){
	 printf("~class B : id=%d\n", mId);
}

void ClassB::func1(void) {
	ClassA::func1();
	printf("B : %s\n", __FUNCTION__);
}

int main(int argc, char *argv[])
{
    ClassB cls_b(1);
	ClassB *cls_b1 = new ClassB(2);

	cls_b.data = 1;
	cls_b1->data = 2;

	printf("data = %d, %d\n", cls_b.data, cls_b1->data);
	
	cls_b1->func1();

	printf("test class end\n");

	delete cls_b1;
	
    return 0;
}

