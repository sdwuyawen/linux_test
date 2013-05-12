/**
   @filename :
   		test_class.cpp
**/
#include <stdio.h>

class ClassA
{
public:
    ClassA();
    virtual ~ClassA();
};

ClassA::ClassA() {
	printf("class A\n");
}

ClassA::~ClassA() {
	printf("~class A\n");
}

class ClassB
{
public:
    ClassB(int id);
    virtual ~ClassB();

public:
	 int data;

private:
	 ClassA mClassA;
	 int mId;
};

ClassB::ClassB(int id) {
	 printf("class B : id=%d\n", id);
	 mId = id;
}

ClassB::~ClassB(){
	 printf("~class B : id=%d\n", mId);
}

int main(int argc, char *argv[])
{
    ClassB cls_b(1);
	ClassB *cls_b1 = new ClassB(2);

	cls_b.data = 1;
	cls_b1->data = 2;

	printf("data = %d, %d\n", cls_b.data, cls_b1->data);

	printf("test class end\n");

	if (cls_b1) {
		 delete cls_b1;
	}
	
    return 0;
}

