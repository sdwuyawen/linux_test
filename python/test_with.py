# -*- coding: utf-8 -*-
"""
Created on Sat Mar 28 20:05:24 2015

@author: paul2

Sample()的__enter__()方法返回新创建的Sample对象，并赋值给变量sample。
实际上，在with后面的代码块抛出任何异常时，__exit__()方法被执行。正如例子所示，异常抛出时，
与之关联的type，value和stack trace传给__exit__()方法，因此抛出的ZeroDivisionError异常被打印出来了。
开发库时，清理资源，关闭文件等等操作，都可以放在__exit__方法当中
"""
class Sample:
    def __enter__(self):
        print "In enter"
        return self
 
    def __exit__(self, type, value, trace):
        print "type:", type
        print "value:", value
        print "trace:", trace
 
    def do_something(self):
        print "do_something"
        bar = 1/0
#        bar = 0
        return bar + 10
 
with Sample() as sample:
    sample.do_something()