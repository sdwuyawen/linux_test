# -*- coding: utf-8 -*-
"""
Created on Thu May 14 09:27:19 2015

@author: paul
@note :
1. fsolve函数可以用来对非线性方程组进行求解. 它的基本调用形式如下：
    fsolve(func, x0)
  func(x)是计算方程组误差的函数，它的参数x是一个矢量，表示方程组的各个未知数的一组可能解，
  func返回将x代入方程组之后得到的误差；x0为未知数矢量的初始值。如果要对如下方程组进行求解的话：
    f1(u1,u2,u3) = 0
    f2(u1,u2,u3) = 0
    f3(u1,u2,u3) = 0

那么func可以如下定义：
  def func(x):
      u1,u2,u3 = x
      return [f1(u1,u2,u3), f2(u1,u2,u3), f3(u1,u2,u3)]
      
下面是一个实际的例子，求解如下方程组的解：
    5*x1 + 3 = 0
    4*x0*x0 - 2*sin(x1*x2) = 0
    x1*x2 - 1.5 = 0

2. 在对方程组进行求解时，fsolve会自动计算方程组的雅可比矩阵，如果方程组中的未知数很多，而与每个方程有关的未知数较少时，即雅可比矩阵比较稀疏时，
    传递一个计算雅可比矩阵的函数将能大幅度提高运算速度

"""
from scipy.optimize import fsolve
from math import sin,cos
def f(x):
    x0 = float(x[0])
    x1 = float(x[1])
    x2 = float(x[2])
    return [
        5*x1+3,
        4*x0*x0 - 2*sin(x1*x2),
        x1*x2 - 1.5
    ]
    
def j(x):
    x0 = float(x[0])
    x1 = float(x[1])
    x2 = float(x[2])    
    return [
        [0, 5, 0],
        [8*x0, -2*x2*cos(x1*x2), -2*x1*cos(x1*x2)],
        [0, x2, x1]
    ]

result = fsolve(f, [1,1,1])
print result
print f(result)
 
result1 = fsolve(f, [1,1,1], fprime=j)
print result1
print f(result1)

