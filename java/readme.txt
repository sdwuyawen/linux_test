1.设置env
	export CLASSPATH=.:$CLASSPATH
	或
	直接修改/etc/profile下的
	export CLASSPATH

	如果没有指定".", 那么需要在命令行中增加"-classpath . "才行。

2.编译
    javac first.java
    
3.运行
    java first     
	或
    javac -classpath . first

	运行过程:
	这个装载工作是由Java类加载器完成的。当JVM启动时，3个类加载器被使用：
	1).引导类加载器：加载位于/ jre / lib目录的核心Java库。这是jvm核心的一部分，并且是原生的代码。
	2).扩展类加载器：加载代码的扩展目录（例如，/jar/ lib / ext目录）。
	3).系统类加载器：在CLASSPATH中找到负载代码。
	所以HelloWorld类是由系统类加载器加载。当执行的主要方法，它会触发加载，链接和其他相关的类的初始化（查看），如果它们存在。
	最后，main()的帧被加载到jvm堆栈,程序计数器（PC）被相应地设置。程序计数器然后指示println()帧的加载到JVM堆栈。
	当main（）方法完成后，它会从堆栈中弹出执行完成。

	流程如下：
	
			 			Link
	Load --> (Verify, Prepare, Resolve) --> Initialize
    
4.其他
	1). 反汇编
	命令：
	javap -c first
	输出如下：
	Compiled from "first.java"
	public class first extends java.lang.Object{
	public first();
	  Code:
	   0:	aload_0
	   1:	invokespecial	#1; //Method java/lang/Object."<init>":()V
	   4:	return

	public static void main(java.lang.String[]);
	  Code:
	   0:	getstatic	#2; //Field java/lang/System.out:Ljava/io/PrintStream;
	   3:	ldc	#3; //String This is my first program.
	   5:	invokevirtual	#4; //Method java/io/PrintStream.println:(Ljava/lang/String;)V
	   8:	return

	}

	aload_0指令是加载一个从栈中引用的本地变量0，
	getstatic 指令获取一个类的静态字段值。注意“＃2” 指令在getstatic指令后指向运行常量池。常量池是一个JVM运行时数据区，
	我们可以用“javap -verbose”命令来查看常量池。
	每个指令开始于一个数字，如0, 1，4等。在.class文件中，每个方法都有一个对应的字节码数组。
	这些数字对应的每一个操作码和它的参数都存储在数组中的索引中。
	每个操作码为1个字节，指令可以有0个或多个参数

	命令：
	javap -verbose first
	输出如下：
	Compiled from "first.java"
	public class first extends java.lang.Object
	  SourceFile: "first.java"
	  minor version: 0
	  major version: 50
	  Constant pool:
	const #1 = Method	#6.#15;	//  java/lang/Object."<init>":()V
	const #2 = Field	#16.#17;	//  java/lang/System.out:Ljava/io/PrintStream;
	const #3 = String	#18;	//  This is my first program.
	const #4 = Method	#19.#20;	//  java/io/PrintStream.println:(Ljava/lang/String;)V
	const #5 = class	#21;	//  first
	const #6 = class	#22;	//  java/lang/Object
	const #7 = Asciz	<init>;
	const #8 = Asciz	()V;
	const #9 = Asciz	Code;
	const #10 = Asciz	LineNumberTable;
	const #11 = Asciz	main;
	const #12 = Asciz	([Ljava/lang/String;)V;
	const #13 = Asciz	SourceFile;
	const #14 = Asciz	first.java;
	const #15 = NameAndType	#7:#8;//  "<init>":()V
	const #16 = class	#23;	//  java/lang/System
	const #17 = NameAndType	#24:#25;//  out:Ljava/io/PrintStream;
	const #18 = Asciz	This is my first program.;
	const #19 = class	#26;	//  java/io/PrintStream
	const #20 = NameAndType	#27:#28;//  println:(Ljava/lang/String;)V
	const #21 = Asciz	first;
	const #22 = Asciz	java/lang/Object;
	const #23 = Asciz	java/lang/System;
	const #24 = Asciz	out;
	const #25 = Asciz	Ljava/io/PrintStream;;
	const #26 = Asciz	java/io/PrintStream;
	const #27 = Asciz	println;
	const #28 = Asciz	(Ljava/lang/String;)V;

	{
	public first();
	  Code:
	   Stack=1, Locals=1, Args_size=1
	   0:	aload_0
	   1:	invokespecial	#1; //Method java/lang/Object."<init>":()V
	   4:	return
	  LineNumberTable: 
	   line 1: 0


	public static void main(java.lang.String[]);
	  Code:
	   Stack=2, Locals=1, Args_size=1
	   0:	getstatic	#2; //Field java/lang/System.out:Ljava/io/PrintStream;
	   3:	ldc	#3; //String This is my first program.
	   5:	invokevirtual	#4; //Method java/io/PrintStream.println:(Ljava/lang/String;)V
	   8:	return
	  LineNumberTable: 
	   line 3: 0
	   line 4: 8


	}

 JVM定义：
	运行常量池提供一个类似于传统的编程语言的符号表函数，尽管它包含了比典型的符号表范围更广的数据
	“invokespecial #1″指令指向#1常量在常量池中.常量是”Method #6.#15;“从数字上看，我们就可以按递归方式来得到最终的常量。
	LineNumberTable提供用来调试java源代码对应字节码的行数信息
	例如，在main方法里Java源代码中第3行对应字节码0，第4行对应字节码8。


5. demo
  1). first.java
	输出调试信息
	
  2). DateMethodsTest.java
	它读取命令行参数指定的类名，然后打印这个类所具有的方法信息
	
  3). ReflectTester.java    
	ReflectTester类有一个copy(Object object)方法，这个方法能够创建一个和参数object同样类型的对象，
	然后把object对象中的所有属性拷贝到新建的对象中，并将它返回这个实例。
	
	ReflectTester 类的copy(Object object)方法依次执行以下步骤：
	(1). 获得对象的类型：
	Class classType=object.getClass();
	System.out.println("Class:"+classType.getName());

	在java.lang.Object 类中定义了getClass()方法，因此对于任意一个Java对象，都可以通过此方法获得对象的类型。
	Class类是Reflection API 中的核心类，它有以下方法：
		getName()：获得类的完整名字；
		getFields()：获得类的public类型的属性；
		getDeclaredFields()：获得类的所有属性；
		getMethods()：获得类的public类型的方法；
		getDeclaredMethods()：获得类的所有方法；

	getMethod(String name, Class[] parameterTypes)：获得类的特定方法，
		name参数指定方法的名字，
		parameterTypes 参数指定方法的参数类型；
	
	(2).构造
	getConstructors()：获得类的public类型的构造方法；
	getConstructor(Class[] parameterTypes)：获得类的特定构造方法，
		parameterTypes 参数指定构造方法的参数类型；
		
	newInstance()：通过类的不带参数的构造方法创建这个类的一个对象；

	Object objectCopy=classType.getConstructor(new Class[]{}).newInstance(new Object[]{});
		以上代码先调用Class类的getConstructor()方法获得一个Constructor 对象，
		它代表默认的构造方法，然后调用Constructor对象的newInstance()方法构造一个实例。

	(3). 获得对象的所有属性：
	Field fields[]=classType.getDeclaredFields();
	Class 类的getDeclaredFields()方法返回类的所有属性，包括public、protected、默认和private访问级别的属性。

	(4). 获得每个属性相应的getXXX()和setXXX()方法，然后执行这些方法，把原来对象的属性拷贝到新的对象中。
	
  4). InvokeTester.java
  
