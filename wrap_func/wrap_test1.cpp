#include <exception>  
#include <new>  
#include <cstdlib>  
#include <cstdio>  
   
void* operator new(size_t size)  
{  
    void *p = malloc(size);  
    
    return p;  
} 

void operator delete(void *ptr)
{
	free(ptr);
}  
