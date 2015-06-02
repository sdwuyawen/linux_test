1.dlmalloc函数内部是通过sbrk()来调整内存空间
  char *sbrk(int incr)
  返回值：函数调用成功返回一指针，指向下一个内存空间。函数调用失败则返回（void*）-1,将errno设为ENOMEM。  
  sbrk(0)返回可用堆的顶端地址， 其参数可以正负数(负数是减少mem的量，正数是增加mem的量)
