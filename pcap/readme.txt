1.libpcap接口
	1).获取网络接口
	首先我们需要获取监听的网络接口：
	我们可以手动指定或让libpcap自动选择，先介绍如何让libpcap自动选择：
	char * pcap_lookupdev(char * errbuf)
	上面这个函数返回第一个合适的网络接口的字符串指针，如果出错，则errbuf存放出错信息字符串，errbuf至少应该是PCAP_ERRBUF_SIZE个字节长度的。注意，很多libpcap函数都有这个参数。
	pcap_lookupdev()一般可以在跨平台的，且各个平台上的网络接口名称都不相同的情况下使用。
	如果我们手动指定要监听的网络接口，则这一步跳过，我们在第二步中将要监听的网络接口字符串硬编码在pcap_open_live里。

	2).释放网络接口
	在操作为网络接口后，我们应该要释放它：
	void pcap_close(pcap_t * p)
	该函数用于关闭pcap_open_live()获取的pcap_t的网络接口对象并释放相关资源。

	3).打开网络接口
	获取网络接口后，我们需要打开它：
	pcap_t * pcap_open_live(const char * device, int snaplen, int promisc, int to_ms, char * errbuf)
	上面这个函数会返回指定接口的pcap_t类型指针，后面的所有操作都要使用这个指针。
	第一个参数是第一步获取的网络接口字符串，可以直接使用硬编码。
	第二个参数是对于每个数据包，从开头要抓多少个字节，我们可以设置这个值来只抓每个数据包的头部，而不关心具体的内容。典型的以太网帧长度是1518字节，但其他的某些协议的数据包会更长一点，但任何一个协议的一个数据包长度都必然小于65535个字节。
	第三个参数指定是否打开混杂模式(Promiscuous Mode)，0表示非混杂模式，任何其他值表示混合模式。如果要打开混杂模式，那么网卡必须也要打开混杂模式，可以使用如下的命令打开eth0混杂模式：
	ifconfig eth0 promisc
	第四个参数指定需要等待的毫秒数，超过这个数值后，第3步获取数据包的这几个函数就会立即返回。0表示一直等待直到有数据包到来。
	第五个参数是存放出错信息的数组。

	4).获取数据包
	打开网络接口后就已经开始监听了，那如何知道收到了数据包呢?有下面3种方法：
	a)
	u_char * pcap_next(pcap_t * p, struct pcap_pkthdr * h)
	如果返回值为NULL，表示没有抓到包
	第一个参数是第2步返回的pcap_t类型的指针
	第二个参数是保存收到的第一个数据包的pcap_pkthdr类型的指针
	pcap_pkthdr类型的定义如下：
		struct pcap_pkthdr  
		{  
		  struct timeval ts;    /* time stamp */  
		  bpf_u_int32 caplen;   /* length of portion present */  
		  bpf_u_int32 len;      /* length this packet (off wire) */  
		};  
		注意这个函数只要收到一个数据包后就会立即返回

	b)
	int pcap_loop(pcap_t * p, int cnt, pcap_handler callback, u_char * user)
	第一个参数是第2步返回的pcap_t类型的指针
	第二个参数是需要抓的数据包的个数，一旦抓到了cnt个数据包，pcap_loop立即返回。负数的cnt表示pcap_loop永远循环抓包，直到出现错误。
	第三个参数是一个回调函数指针，它必须是如下的形式：
	void callback(u_char * userarg, const struct pcap_pkthdr * pkthdr, const u_char * packet)
	第一个参数是pcap_loop的最后一个参数，当收到足够数量的包后pcap_loop会调用callback回调函数，同时将pcap_loop()的user参数传递给它
	第二个参数是收到的数据包的pcap_pkthdr类型的指针
	第三个参数是收到的数据包数据

	5).分析数据包
	我们既然已经抓到数据包了，那么我们要开始分析了，这部分留给读者自己完成，具体内容可以参考相关的网络协议说明。在本文的最后，我会示范性的写一个分析arp协议的sniffer，
	仅供参考。要特别注意一点，网络上的数据是网络字节顺序的，因此分析前需要转换为主机字节顺序(ntohs()函数)。

	6).过滤数据包
	我们抓到的数据包往往很多，如何过滤掉我们不感兴趣的数据包呢?
	几乎所有的操作系统(BSD, AIX, Mac OS, Linux等)都会在内核中提供过滤数据包的方法，主要都是基于BSD Packet Filter(BPF)结构的。libpcap利用BPF来过滤数据包。
	过滤数据包需要完成3件事：
	a) 构造一个过滤表达式
	b) 编译这个表达式
	c) 应用这个过滤器

	a)
	BPF使用一种类似于汇编语言的语法书写过滤表达式，不过libpcap和tcpdump都把它封装成更高级且更容易的语法了，具体可以man tcpdump，以下是一些例子：

	src host 192.168.1.177
	只接收源ip地址是192.168.1.177的数据包

	dst port 80
	只接收tcp/udp的目的端口是80的数据包

	not tcp
	只接收不使用tcp协议的数据包

	tcp[13] == 0x02 and (dst port 22 or dst port 23)
	只接收SYN标志位置位且目标端口是22或23的数据包（tcp首部开始的第13个字节）

	icmp[icmptype] == icmp-echoreply or icmp[icmptype] == icmp-echo
	只接收icmp的ping请求和ping响应的数据包

	ether dst 00:e0:09:c1:0e:82
	只接收以太网mac地址是00:e0:09:c1:0e:82的数据包

	ip[8] == 5
	只接收ip的ttl=5的数据包（ip首部开始的第8个字节）

	b)
	构造完过滤表达式后，我们需要编译它，使用如下函数：
	int pcap_compile(pcap_t * p, struct bpf_program * fp, char * str, int optimize, bpf_u_int32 netmask)
	fp：这是一个传出参数，存放编译后的bpf
	str：过滤表达式
	optimize：是否需要优化过滤表达式
	metmask：简单设置为0即可

	c)
	最后我们需要应用这个过滤表达式：
	int pcap_setfilter(pcap_t * p,  struct bpf_program * fp)
	第二个参数fp就是前一步pcap_compile()的第二个参数

	c)
	int pcap_dispatch(pcap_t * p, int cnt, pcap_handler callback, u_char * user)
	这个函数和pcap_loop()非常类似，只是在超过to_ms毫秒后就会返回(to_ms是pcap_open_live()的第4个参数)

	7).
	int pcap_lookupnet(const char * device, bpf_u_int32 * netp, bpf_u_int32 * maskp, char * errbuf)
	可以获取指定设备的ip地址，子网掩码等信息
	netp：传出参数，指定网络接口的ip地址
	maskp：传出参数，指定网络接口的子网掩码
	pcap_lookupnet()失败返回-1
	我们使用inet_ntoa()将其转换为可读的点分十进制形式的字符串

2.
 1).可以先清空arp缓存，这样就可以看到整个通信过程（包括一开始的arp广播）
	在客户机上运行下列命令来清空记录服务器的arp缓存：
	sudo arp -d 192.168.56.101
	arp -a
	后发现已经删除了记录服务器的arp缓存
