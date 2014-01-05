import java.lang.reflect.Method;

public class DateMethodsTest {     
  public static void main(String args[]) throws Exception {    
	Class<?> classType;
	String className = "java.util.Date";
	
    // 加载并初始化命令行参数指定的类     
    if (args.length != 0) {
		className = args[0];
	} 
	
	System.out.println("num = " + args.length + " class = " + className); 
	classType = Class.forName(className);
	
    // 获得类的所有方法     
    Method methods[] = classType.getDeclaredMethods();     
    for (int i = 0; i < methods.length; i++) {     
      System.out.println(methods[i].toString());     
    }     
  }     
}   
