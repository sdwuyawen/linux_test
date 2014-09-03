1. 如果把all clean:这一段mask掉，改为include mk1/Android.mk的输出：
======== /home/paul2/work/test/linux_test/makefile_test/mk1/Android.mk =======
======== /home/paul2/work/test/linux_test/makefile_test/mk1/Android.mk =======
Android.mk mk1/Android.mk
mk1

那么现在$(MAKEFILE_LIST), 为Android.mk mk1/Android.mk, 而当前目录为/home/paul2/work/test/linux_test/makefile_test，
$(lastword $(MAKEFILE_LIST))为mk1/Android.mk

2. eval函数
函数原型 $(eval text)
	它的意思是text的内容将作为makefile的一部分而被make解析和执行
	
	(1). 
	$(eval xd:xd.c a.c)

	将会产生一个这样的编译
	cc   xd.c a.c -o xd

	(2).
	define MA
	aa:aa.c
	 gcc  -g -o aa aa.c
	endef 

	$(eval $(call MA) )
	
	会产生一个这样的编译：
	gcc -g -o aa aa.c

	(3).
	OBJ=a.o b.o c.o d.o main.o

	define MA
	main:$(OBJ)
	 gcc  -g -o main $$(OBJ)
	endef 

	$(eval $(call MA) )

	会产生这样的编译过程：
	cc -c -o a.o a.c
	cc -c -o b.o b.c
	cc -c -o c.o c.c
	g++ -c -o d.o d.cpp
	cc -c -o main.o main.c
	gcc -g -o main a.o b.o c.o d.o main.o

	请注意到$$(OBJ) ,因为make要把这个作为makefile的一行，要让这个地方出现$,就要用两个$,因为两个$,make才把把作为$字符。
