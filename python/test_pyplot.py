# -*- coding: utf-8 -*-
"""
Created on Mon Apr 27 16:14:39 2015

@author: paul

@note :
1. matplotlib所绘制的图的每个组成部分都对应有一个对象，我们可以通过调用这些对象的属性设置方法set_*或者pyplot的属性设置函数setp设置其属性值

2. 可以通过调用Line2D对象的get_*方法，或者plt.getp函数获取对象的属性值

3. 一个绘图对象(figure)可以包含多个轴(axis)，在Matplotlib中用轴表示一个绘图区域，可以将其理解为子图。上面的第一个例子中，绘图对象只包括一个轴，
  因此只显示了一个轴(子图)。我们可以使用subplot函数快速绘制有多个轴的图表。subplot函数的调用形式如下：
	subplot(numRows, numCols, plotNum)
  subplot将整个绘图区域等分为numRows行 * numCols列个子区域，然后按照从左到右，从上到下的顺序对每个子区域进行编号，左上的子区域的编号为1。
  如果numRows，numCols和plotNum这三个数都小于10的话，可以把它们缩写为一个整数，例如subplot(323)和subplot(3,2,3)是相同的。
  subplot在plotNum指定的区域中创建一个轴对象。如果新创建的轴和之前创建的轴重叠的话，之前的轴将被删除。
  
  如果希望某个轴占据整个行或者列的话，可以如下调用subplot：
	plt.subplot(221) # 第一行的左图
	plt.subplot(222) # 第一行的右图
	plt.subplot(212) # 第二整行
	plt.show()

"""
import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(0, 10, 1000)
y = np.sin(x)
z = np.cos(x**2)

plt.figure(figsize=(8,4))	#调用figure创建一个绘图对象，并且使它成为当前的绘图对象
plt.plot(x,y,label="$sin(x)$",color="red",linewidth=2)
plt.plot(x,z,"b--",label="$cos(x^2)$")	#其中b表示蓝色，"--"表示线型为虚线
plt.xlabel("Time(s)")
plt.ylabel("Volt")
plt.title("PyPlot First Example")
plt.ylim(-1.2,1.2)	#y轴的范围[-1.2, 1.2]
plt.legend()	#显示图示
plt.show()

