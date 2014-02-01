/**
 * 	@filename :
 * 		test_coroutine.c
 * 
 **/ 
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
