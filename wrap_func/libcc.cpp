#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC_SIZE		256

extern "C" void libcc_printf(char *str);

class ClassA {
public:	
	ClassA(int n);
	~ClassA();
	
	int getCnt(void);
	
private:
	int m_cnt;	
};

ClassA::ClassA(int n): m_cnt(n) {
	 printf("class A\n");
}

ClassA::~ClassA() {
	 printf("~class A\n");
}

int ClassA::getCnt() {
	return m_cnt;
}

void libcc_printf(char *str)
{
	char *ptr = new char[strlen(str) + MALLOC_SIZE];
	
	if (ptr != NULL) {
		sprintf(ptr, "%s : %s", __func__, str);
		printf("%s\n", ptr);
		delete [] ptr;
	}
	
	printf("new ClassA\n");
	ClassA *a = new ClassA(3);
	if (a != NULL) {
		printf("cnt %d\n", a->getCnt());
		delete a;
	}
	
	return;
}
