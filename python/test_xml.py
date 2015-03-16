# -*- coding: utf-8 -*-
"""
Created on Sun Mar 15 18:10:23 2015

@author: paul2
"""
# import os
import sys
from xml.etree import ElementTree as ET

def print_node(node):
    '''打印结点基本信息'''
    print "=============================================="
    print "node.attrib:%s" % node.attrib
    if node.attrib.has_key("type") > 0:
        print "node.attrib['type']:%s" % node.attrib['type']
    print "node.tag:%s" % node.tag
    print "node.text:%s" % node.text
    return

def read_xml(text):
    '''读xml文件'''
    # 加载XML文件（2种方法,一是加载指定字符串，二是加载指定文件）   
    # root = ElementTree.parse(r"D:/config.xml")
    root = ET.fromstring(text)
     
    # 获取element的方法
    # 1 通过getiterator
    lst_node = root.getiterator("Widget")
    for node in lst_node:
        print_node(node)
         
    # 2通过 getchildren
    lst_node_child = lst_node[0].getchildren()[0]
    print_node(lst_node_child)
         
    # 3 .find方法
    print "=== find ==="
    node_find = root.find('Pageinfo/Page/Widget')
    print "len =", len(node_find), "type =", type(node_find)
    print_node(node_find)
    print "print child"
    child_nodes = node_find.getchildren()
    for child_node in child_nodes:
        print_node(child_node)
    print "\n"

    #4. findall方法
    print "=== find all ==="
    node_findall = root.findall("Pageinfo/Page/Widget/Iconname")
    print "len =", len(node_findall), "type =", type(node_findall)
    print_node(node_findall[1])
    
    return
    
if __name__ == '__main__':
    read_xml(open(sys.argv[1]).read())
