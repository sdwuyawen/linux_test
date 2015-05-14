# -*- coding: utf-8 -*-
"""
Created on Thu May 14 08:53:52 2015

@author: paul
@note 
1. sudo pip install traits
2.
 1). Str 输入字符串
 2). Int 输入数字
 3). 由于salary属性定义为Int类型，所以它将检查非法输入，并以红色背景警示
 4). 用traitsui库创建的界面可以选择后台界面库，目前支持的有qt4和wx两种。
 在启动程序时添加 -toolikt qt4 或者 -toolikt wx 选择使用何种界面库生成界面。默认使用wx作为后台界面库
 
3. Item对象
Item对象是视图的基本组成单位，每个Item描述界面中的中的一个控件，通常都是用来显示HasTraits对象中的某一个trait属性。
每个Item由一系列的关键字参数来进行配置，这些参数对Item的内容、表现以及行为进行描述。其中最重要的一个参数就是name。
我们看到name参数的值都配置为SimpleEmployee类的trait属性名，于是Item就知道到哪里去寻找真正要显示的值了。
可以看出视图与数据是通过属性名联系起来的。剩下的两个参数label和tooltip设置Item在界面中的一些显示相关的属性。
Item对象还有很多属性其它属性，请参考TraitsUI的用户手册，或者在iPython中输入Item??直接查看其源代码

4. Group对象 (界面中表示标签页)
我们通过把三个Item对象传递给View，创建了一个控件垂直排列的布局。
然而在真正的界面开发中，需要更加高级的布局方式，例如，将一组相关的元素组织在一起，放到一个组中，我们可以为此组添加标签，
定义组的帮助文本，通过设置组的属性使组类的元素同时有效或无效
1).
如果我们希望能同时看到两个Group的话，可以另外再创建一个Group将这两个Group包括起来：

view2 = View( Group( view1.content ) )

这里我们创建视图view2，它包括一个Group，此Group的内容则直接使用view1的内容(也就是那两个Group)。当然也可以把view1中的内容复制进去：

view2 = View(Group(
    Group(
        Item(name = 'employee_number', label=u'编号'),
        Item(name = 'department', label=u"部门", tooltip=u"在哪个部门干活"),
        Item(name = 'last_name', label=u"姓"),
        Item(name = 'first_name', label=u"名"),
        label = u'个人信息',
        show_border = True
        ),
    Group(
        Item(name = 'salary', label=u"工资"),
        Item(name = 'bonus', label=u"奖金"),
        label = u'收入',
        show_border = True
        )
))
...
sam.configure_traits(view=view2)

5. HSplit对象

"""

from traits.api import HasTraits, Str, Int
from traitsui.api import View, Item,Group

class SimpleEmployee(HasTraits):
    first_name = Str
    last_name = Str
    department = Str

    employee_number = Str
    salary = Int
    bonus = Int

view1 = View(
    Group(
        Item(name = 'employee_number', label=u'编号'),
        Item(name = 'department', label=u"部门", tooltip=u"在哪个部门干活"),
        Item(name = 'last_name', label=u"姓"),
        Item(name = 'first_name', label=u"名"),
        label = u'个人信息',
        show_border = True
    ),
    Group(
        Item(name = 'salary', label=u"工资"),
        Item(name = 'bonus', label=u"奖金"),
        label = u'收入',
        show_border = True
    )
)

sam = SimpleEmployee()
sam.configure_traits(view=view1)