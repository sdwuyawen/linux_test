/**
 * @filename 
 * 	test_c_cxx.h
 * 
 **/
#ifndef __TEST_C_CXX_H__
#define __TEST_C_CXX_H__

struct test_c_cxx_app {
	void *userData;
	void (*onAppCmd)(struct test_c_cxx_app* app, int cmd);
};

#ifdef __cplusplus
extern "C" {
#endif

int test_c_cxx_enter(struct test_c_cxx_app *app);
int test_c_cxx_exit(struct test_c_cxx_app *app);

#ifdef __cplusplus
}
#endif


#endif //__TEST_C_CXX_H__

