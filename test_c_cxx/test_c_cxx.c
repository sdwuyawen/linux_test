/**
 * @filename :
 * 	test_c_cxx.c
 * 
 **/
#include <stdio.h>

#include "test_c_cxx.h"

int main(int argc, char *argv[]) 
{
	int ret;
	struct test_c_cxx_app app;
	
	memset(&app, 0, sizeof(app));
	
	ret = test_c_cxx_enter(&app);
	
	app.onAppCmd(&app, 0);
	
	ret = test_c_cxx_exit(&app);
	
	return 0;
}
