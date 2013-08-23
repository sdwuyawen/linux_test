/**
 * @filename :
 * 	test_c_cxx_api.h
 * 
 **/
#ifndef __TEST_C_CXX_API_H__
#define __TEST_C_CXX_API_H__

#include "test_c_cxx.h"

class CEventLoop
{
public:
  CEventLoop(test_c_cxx_app* application);
  
private:
  static void cmdCallback(test_c_cxx_app* application, int command);
  //void cmdCallback(test_c_cxx_app* application, int command);
  
  test_c_cxx_app* m_application;  
};

#endif //__TEST_C_CXX_API_H__
