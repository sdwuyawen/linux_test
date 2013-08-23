/**
 * 	@filename :
 * 		test_c_cxx_api.cpp
 * 
 **/  
#include <stdio.h> 

#include "test_c_cxx_api.h"

CEventLoop::CEventLoop(test_c_cxx_app* application)
	: m_application(application)
{
	if (m_application == NULL)
		return;
	
	m_application->userData = this;
	m_application->onAppCmd = cmdCallback;
}
 
void CEventLoop::cmdCallback(test_c_cxx_app* application, int command)
{
	if (application == NULL || application->userData == NULL)
		return;
	
	CEventLoop& eventLoop = *((CEventLoop*)application->userData);
	
	printf("cmd : %d\n", command);
}


int test_c_cxx_enter(struct test_c_cxx_app *app)
{
	CEventLoop* eventloop = new CEventLoop(app);
	
	printf("%s\n", __FUNCTION__);
		
	return 0;
}

int test_c_cxx_exit(struct test_c_cxx_app *app) 
{
	if (app->userData != NULL) {
		CEventLoop* eventloop = (CEventLoop*)app->userData;
		delete eventloop;
		app->userData = NULL;
	}
	
	printf("%s\n", __FUNCTION__);
	
	return 0;
}
