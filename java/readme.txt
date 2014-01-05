1.设置env
	export CLASSPATH=.:$CLASSPATH
或
	直接修改/etc/profile下的
	export CLASSPATH

2.编译
    javac first.java
    
3.运行
    java first     
    
    
4. demo
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
  
