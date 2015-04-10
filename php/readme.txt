1. html <input>
  type类型：  
    button
    checkbox
    file
    hidden
    image
    password
    radio
    reset
    submit
    text

  1). text
  <form action="user_text.php" method="get">
    <input type="text" name="fname" />
    <input type="text" name="lname" />
	<br />		
	<input type="submit" name="submit" value="Submit" />
  </form>		
  
  php访问通过$_GET["fname"], $_GET["lname"]来访问上传的值
  user_text.php
  
  2). password
  定义密码字段（密码字段中的字符会被遮蔽）：
	<input type="password" name="password" />
	
  php访问通过$_GET["password"]来访问上传的值
  password.php

  3). button
	定义可点击的按钮，在用户单击按钮时启动一段 JavaScript：
	<input type="button" value="Click me" onclick="msg()">   
	
  4). checkbox  
	<input type="checkbox" name="vehicle[]" value="Bike"> I have a bike<br>
	<input type="checkbox" name="vehicle[]" value="Car"> I have a car<br>   
	
  5). file
  定义文件选择字段和 "浏览..." 按钮，供文件上传：
	<form action="upload_file.php" method="post" enctype="multipart/form-data">
		<input type="file" name="file"/> 
		<input type="file" name="image"/>
		<input type="submit" name="submit" value="Submit" />
	</form>
	
	php访问通过$_FILES["file"], $_FILES["image"]来访问上传的文件
	upload_file.php
  
  6). hidden
	定义隐藏字段，隐藏字段对于用户是不可见的。隐藏字段常常存储默认值，或者由 JavaScript 改变它们的值：
	<input type="hidden" name="country" value="Norway">   
	
  7). image
	定义图像作为提交按钮：
	<input type="image" src="img_submit.gif" alt="Submit">   
	
  8). radio
	允许用户在一定数量的选择中选取一个选项：
	<input type="radio" name="gender" value="male"> Male<br>
	<input type="radio" name="gender" value="female"> Female<br>   
	
	php通过$_GET["gender"]来访问， 值为male或female.
	
  9). reset
	定义重置按钮（重置所有表单值为默认值）：
	<input type="reset">
    
  10). submit
	<input type="submit" name="submit" value="Submit" />
	php通过$_GET["submit"] or $_POST["submit"]来访问value， 
=================================================================================
2.
=================================================================================
HTML 中 id与name 区别
一个name可以同时对应多个控件，比如checkbox和radio
而id必须是全文档中唯一的

id的用途 
 1） id是HTML元素的Identity，主要是在客户端脚本里用。
 2） label与form控件的关联，如
	<label for="MyInput">My Input</label>
	<input id="MyInput" type="text">
	for属性指定与label关联的元素的id，不可用name替代

 3）脚本中获得对象：IE支持在脚本中直接以id（而不是name）引用该id标识的对象。

 例如上面的input，要在脚本中获得输入的内容，可以直接以 MyInput.value来获得。如果用DOM的话，则用document.getElementById("MyInput").value；
 如果要用name的话，通常先得到包含控件的form，例如document.forms[0]，然后从form再引用name，注意这样得到的是经过计算后将发送给服务器的值

name的用途
 1）主要是用于获取提交表单的某表单域信息， 作为可与服务器交互数据的HTML元素的服务器端的标示，比如input、select、textarea、框架元素(iframe、frame、 window的名字，
 用于在其他frame或window指定target )和button等，这些元素都与表单(框架元素作用于form的target)提交有关，浏览器会根据name来设定发送到服务器的request， 
 在表单的接收页面只接收有name的元素,  所以赋ID的元素通过表单是接收不到值的。 我们可以在服务器端根据其Name通过Request.Params取得元素提交的值。
 在form里面，如果不指定name，就不会发送到服务器端。
 2）HTML元素Input type='radio'分组，我们知道radio button控件在同一个分组类，check操作是mutex的，同一时间只能选中一个radio，这个分组就是根据相同的name属性来实现的。 
 3）建立页面中的锚点，我们知道<a href="URL">link</a>是获得一个页面超级链接，如果不用href属性，而改用name，如：<a name="PageBottom"></a>，我们就获得了一个页面锚点，
  如
  <strong><a name="1" id="1"></a>Experience (XP)</strong>
  详见 示例
 4）作为对象的Identity，如Applet、Object、Embed等元素。比如在Applet对象实例中，我们将使用其name来引用该对象。
 5）在IMG元素和MAP元素之间关联的时候，如果要定义IMG的热点区域，需要使用其属性usemap，使usemap="#name"(被关联的MAP元素的Name)。
 6）某些特定元素的属性，如attribute，meta和param。例如
 为Object定义参数
 <PARAM NAME = "appletParameter" VALUE = "value">
 或Meta中
 <META NAME = "Author" CONTENT = "Dave Raggett">。

 当然HTML元素的name属性在页面中也可以起那么一点ID的作用，因为在DHTML对象树中，我们可以使用document.getElementsByName来获取一个包含页面中所有指定Name元素的对象数组。
 Name属性还有一个问题，当我们动态创建可包含Name属性的元素时，不能简单的使用赋值element.name = "..."来添加其Name，而必须在创建Element时，
 使用document.createElement('<element name = "myName"></element>')为元素添加Name属性。这是什么意思啊？看下面的例子就明白了。
 <script language ="JavaScript"> 
  var  input = document.createElement('INPUT');
  input.id = 'myId';
  input.name = 'myName';
  alert(input.outerHTML);
 </script >
 消息框里显示的结果是：<INPUT id=myId>
 
 <script language ="JavaScript"> 
  var input = document.createElement(' < INPUT name = " myName " > ');
  input.id = 'myId';
  alert(input.outerHTML);
 </script >
消息框里显示的结果是：<INPUT id=myId name=myName>

name与id 区别 
 id要符合标识的要求，比如大小写敏感，最好不要包含下划线（因为不兼容CSS）。而name基本上没有什么要求，甚至可以用数字。
 table、tr、td、div、p、span、h1、li等元素一般用id。与表单相关的元素也可以赋ID值,  但为这些元素赋ID值的时候引用这些元素的方法就要变一下了，具体的如下： 
  赋name时，引用元素的方式:  document.formName.inputName 或 document.frames("frameName") 
  赋id时，引用元素的方式:  document.all.inputID 或 document.all.frameID 
  除去与表单相关的元素，只能赋id不能赋name，这些元素有body、li、a、table、tr、td、th、p、div、span、pre、dl、dt、dd、font、b等等

CSS 中 id、class属性的区别   
1）class是设置标签的类，用于指定元素属于何种样式的类。在CSS样式中以小写的“点”及“.”来命名，在html页面里则以class="css5" 来选择调用，命名好的CSS又叫css选择器。
 如： .css5{属性：属性值;} 选择器在html调用为“<div class="css5">我是class例子</div>
　　.baobao { color: lime; background: #ff80c0 }
　使用方法：class="baobao" 

2）id是设置标签的标识。用于定义一个元素的独特的样式。在CSS样式定义的时候 以“#”来开头命名id名称
 如一个CSS规则：
  　#binbin { font-size: larger }
　  使用方法：id="binbin" 
 id是一个标签，用于区分不同的结构和内容，就象你的名字，如果一个屋子有2个人同名，就会出现混淆；

3）css里的id用法与class用法一样，只是把class换成id。如例子：
	在CSS样式定义ID 
	 #css5 {height:25px; width: 200px;}	 
	调用ID ：
	 <div id="css5">我是ID例子</div>
	class是一个样式，可以套在任何结构和内容上，就象一件衣服。

4）概念上说就是不一样
id是先找到结构/内容，再给它定义样式；class是先定义好一种样式，再套给多个结构/内容。
 （1） 一个class是用来根据用户定义的标准对一个或多个元素进行定义的。打个比较恰当的比方就是剧本：一个class可以定义剧本中每个人物的故事线，
	你可以通过cSS，javascript等来使用这个类。因此你可以在一个页面上使用class="Frodo" ，class= "Gandalf"， class="Aragorn"来区分不同的故事线。
	还有一点非常重要的是你可以在一个文档中使用任意次数的class。
 （2） id 通常用于定义页面上一个仅出现一次的标记。在对页面排版进行结构化布局时（比如说通常一个页面都是由一个页眉，一个报头< masthead>，一个内容区域和一个页脚等组成），
    一般使用ID比较理想，因为一个ID在一个文档中只能被使用一次。而这些元素在同一页面中很少会出现大于一次的情况。
  归纳成一句话就是：class可以反复使用而id在一个页面中仅能被使用一次。

有可能在很大部分浏览器中反复使用同一个id不会出现问题，但在标准上这绝对是错误的使用，而且很可能导致某些浏览器的现实问题。
在实际应用的时候，class可能对文字的排版等比较有用，而id则对宏观布局和设计放置各种元素较有用。

<<id具有唯一性，而class是一个类，适用于可多次重复使用的容器>>
 
关于ID和Name的一些注意事项
 当然HTML元素的name属性在页面中也可以起那么一点id的作用，因为在DHTML对象树中，我们可以使用document.getElementsByName来获取一个包含页面中所有指定name元素的对象数组。
 如果页面中有n(n>1)个HTML元素的id都相同了怎么办？在DHTML对象中怎么引用他们呢？
 如果我们使用ASPX页面，这样的情况是不容易发生的，因为asp.net进程在处理aspx页面时根本就不允许有ID非唯一，这是页面会被抛出异常而不能被正常的render。
 但如果不是动态页面，我们硬要让id重复那IE怎么做呢？
 这个时候我们还是可以继续使用document.getElementById获取对象，只不过我们只能获取id重复的那 些对象中在HTML Render时第一个出现的对象。
 而这时重复的id会在引用时自动变成一个数组，id重复的元素按Render的顺序依次存在于数组中，数组的脚下标依次表示id出现的先后顺序。
