/**
   @filename :
   		test_class1.cpp
   
   @note :
	输出：
	1). 只有A m_a, 没有int m_i
		A
		B
		A size = 1
		B size = 1, 1
		~B
		~A
	2). 没有A m_a, 只有int m_i
		A
		B
		A size = 1
		B size = 4, 4
		~B
		~A
	3). 同时A m_a和int m_i
		A
		B
		A size = 1
		B size = 8, 8
		~B
		~A
	4).在class A中添加int m_i
		A
		B
		A size = 4
		B size = 8, 8
		~B
		~A	
	5).在class A中再添加int m_i
		A
		B
		A size = 12
		B size = 16, 16
		~B
		~A			
**/
#include <stdio.h>

class A
{
public:
	A(){printf("A\n");};
	~A(){printf("~A\n");};
	
	int m_i;
	int m_array[2];
};

class B
{
public:
	B(){printf("B\n");};
	~B(){printf("~B\n");};

	A m_a;
	int m_i;
};

int main(int argc, char* argv[])
{
	B* m_b = new B();			//同时m_a也先构造
	
	printf("A size = %d\n", sizeof(A));
	printf("B size = %d, %d\n", sizeof(*m_b), sizeof(B));

	delete m_b;
	
	return 0;
}
