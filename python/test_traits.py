# -*- coding: utf-8 -*-
"""
Created on Thu May 14 09:16:57 2015

@author: paul
"""

from traits.api import Delegate, HasTraits, Instance, Int, Str

class Parent ( HasTraits ):
    # 初始化: last_name被初始化为'Zhang'
    last_name = Str( 'Zhang' )

class Child ( HasTraits ):
    age = Int

    # 验证: father属性的值必须是Parent类的实例
    father = Instance( Parent )

    # 委托: Child的实例的last_name属性委托给其father属性的last_name
    last_name = Delegate( 'father' )

    # 监听: 当age属性的值被修改时，下面的函数将被运行
    def _age_changed ( self, old, new ):
        print 'Age changed from %s to %s ' % ( old, new )

p = Parent()
c = Child()
c.father = p
c.configure_traits();        