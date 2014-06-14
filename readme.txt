1). conf_test
	
2). iostat.c
	iostat工具
	
3). malloc_test.c
	测试malloc()和mlock()函数
	
4). pulse-test
	测试pulseaudio接口
	
5). test1_class.cpp
	测试类的继承

6). test_addrinfo.c
	测试getaddrinfo等函数的调用

7). test_atomic.c
    测试原子操作函数

8). test_builtin_clz.c
	测试builtin等函数

9). test.c
	测试printf("%s\n", lable ? : "?");
	打印lable的内容。

10). test_class.cpp
	测试类的继承

11). test_devfs.c
	 test_getprop.c
	测试通过创建/dev/__properties__, 父子进程共享内存。
	
12). test_eeprom.c
	测试eeprom

13). test_event.c
	测试uevent

14). test_fork.c
	测试fork和prctl(PR_SET_NAME, "test_child", NULL, NULL, NULL);

15). test_getopt.c
	测试getopt函数

16). test_glob.c
	测试glob函数
	
17). test_i2c.c
	测试i2c接口

18). test_inotify.c
	测试inotify接口
	 
19). test_likely.c
	测试likely和unlikely
	
20). test_popcnt.c
	测试popcnt

21). test_pthread1.cpp
	测试pthread_once

22). test_ringbuffer.c
	测试ringbuffer

23). test_serial.c
	测试tty

24). test_signal
	测试signal接口

25). test_so
	测试dlopen, dlsym等函数, 访问so里的变量
	 
26). test_so1
	测试父子进程对so的调用
	
27). test_tls.c
	测试tls (pthread_key_create等接口)
	
28). test_tty.c
	测试tty访问接口
	
29). shell
	测试shell脚本
	
30). test_cast.cpp
	测试static_cast, dynamic_cast, reinterpret_cast, const_cast
	
31). test_func_name.c
	定义常量: #define _func	test_func
	替换文件中_func所有标号, 编译出来的看到只有test_func。
	
32). test_c_cxx
    测试c中调用类中的接口（静态接口).	
	如果要调用类实例的其他接口需要通过静态接口来调用才行。	
	
33). test_sizeof.c
    测试不同类型的sizeof

34). test_access.c
	测试access()函数和shell的颜色打印输出。
	
35). test_net.c
	测试net event
	
36). test_file_mem.c
	测试读写父子进程的内存(在父进程创建的内存(分别通过map file和malloc分配的内存))，
	如果是map file, 父子进程可以读写相同的内存(不确定是否安全)。
	如果是malloc, 父子进程读写相同的内存时, 会出现write copy问题。
	
37). test_intersect.c	
	测试rect intersect
	
38). gtk-test
    增加gtk test	
    
    
39). top_ext.c
    top功能扩展    
    
40). test_threads_pool.c
    增加线程池的实现demo       
    
41). test_inherit.cpp
	测试继承，虚函数调用
	
42). test_getpwnam.c
	获取用户属性
	
43). test_pid_ns.c
    测试接口clone和namespace

44). test_sock_pair0.c test_sock_pair.c
    测试socketpair. 创建的socketpair的句柄都有读写权限.

45). test_coroutine.c
	实现yield功能
	
46). sock_br_server.c, sock_br_client.c
	实现通过socket AF_INET来进程通信

47). sock_local_server.c, sock_local_client.c
	实现通过socket AF_LOCAL来进程通信
	命名socket。这里面有一个很关键的东西，socket进程通信命名方式有两种。
	(1).普通的命名，socket会根据此命名创建一个同名的socket文件，客户端连接的时候通过读取该socket文件连接到socket服务端。这种方式的弊端是服务端必须对socket文件的路径具备写权限，
	客户端必须知道socket文件路径，且必须对该路径有读权限。
	(2).抽象命名空间，这种方式不需要创建socket文件，只需要命名一个全局名字，即可让客户端根据此名字进行连接。后者的实现过程与前者的差别是，
	后者在对地址结构成员sun_path数组赋值的时候，必须把第一个字节置0，即sun_path[0] = 0，下面用代码说明：

	第一种方式：
    //name the server socket   
    server_addr.sun_family = AF_UNIX;  
    strcpy(server_addr.sun_path,SERVER_NAME);  
    server_len = sizeof(struct sockaddr_un);  
    client_len = server_len;  


	第二种方式：
    //name the socket  
    server_addr.sun_family = AF_UNIX;  
    server_addr.sun_path[0] = 0;  
    strcpy(server_addr.sun_path+1, SERVER_NAME);  
    //server_len = sizeof(server_addr);  
    server_len = strlen(SERVER_NAME) + offsetof(struct sockaddr_un, sun_path) + 1;  

	其中，offsetof函数在#include <stddef.h>头文件中定义。因第二种方式的首字节置0，我们可以在命名字符串SERVER_NAME前添加一个占位字符串，例如:
    #define SERVER_NAME @socket_server  
    
	int makeAddr(const char* name, struct sockaddr_un* pAddr, socklen_t* pSockLen)
	{
		int nameLen = strlen(name);
		if (nameLen >= (int) sizeof(pAddr->sun_path) -1)  /* too long? */
			return -1;
		pAddr->sun_path[0] = '\0';  /* abstract namespace */
		strcpy(pAddr->sun_path+1, name);
		pAddr->sun_family = AF_UNIX;
		*pSockLen = 1 + nameLen + offsetof(struct sockaddr_un, sun_path);
		return 0;
	}
	
	像下面这样使用这个函数:
	makeAddr("server_socket", &server_addr, &server_len);  
	
48). test_setsid.c 
