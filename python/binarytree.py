# -*- coding: utf-8 -*-
class Queue:
    def __init__(self,size = 16):
        self.queue = []
        self.size = size
        self.front = 0
        self.rear = 0
    def isEmpty(self):
        return self.rear == 0
    def isFull(self):
        if (self.front - self.rear + 1) == self.size:
            return True
        else:
            return False
    def first(self):
        if self.isEmpty():
            raise Exception("QueueIsEmpty")
        else:
            return self.queue[self.front]
    def last(self):
        if self.isEmpty():
            raise Exception("QueueIsEmpty")
        else:
            return self.queue[self.rear]
    def add(self,obj):
        if self.isFull():
            raise Exception("QueueOverFlow")
        else:
            self.queue.append(obj)
            self.rear += 1
    def delete(self):
        if self.isEmpty():
            raise Exception("QueueIsEmpty")
        else:
            self.rear -= 1
            return self.queue.pop(0)
    def show(self):
        print(self.queue)
        
class BinaryTreeNode:
    def __init__(self,data,left,right):
        self.left = left
        self.data = data
        self.right = right
class BinaryTree:
    def __init__(self):
        self.root = None
    def makeTree(self,data,left,right):
        self.root = BinaryTreeNode(data,left,right)
        #left.root = right.root = None
    def isEmpty(self):
        if self.root is None:
            return True
        else:
            return False
    def preOrder(self,r):
        if r.root is not None:
            print(r.root.data)
            if r.root.left is not None:
                self.preOrder(r.root.left)
            if r.root.right is not None:
                self.preOrder(r.root.right)
    def inOrder(self,r):
        if r.root is not None:
            if r.root.left is not None:
                self.inOrder(r.root.left)
            print(r.root.data)
            if r.root.right is not None:
                self.inOrder(r.root.right)
    def postOrder(self,r):
        if r.root is not None:
            if r.root.left is not None:
                self.preOrder(r.root.left)
            if r.root.right is not None:
                self.preOrder(r.root.right)
            print(r.root.data)
    def levelOrder(self,a):
        q = Queue()
        r = a
        while r is not None:
            print(r.root.data)
            if r.root.left is not None:
                q.add(r.root.left)
            if r.root.right is not None:
                q.add(r.root.right)
            if q.isEmpty():
                print("empty")
                r = None
            else:
                r = q.delete()
            


r = BinaryTree()
ra = BinaryTree()
ra.makeTree(2,None,None)
rb = BinaryTree()
rb.makeTree(3,None,None)
r.makeTree(1,ra,rb)
print("前序遍历")
r.preOrder(r)
print("中序遍历")
r.inOrder(r)
print("后序遍历")
r.postOrder(r)
print("层级遍历")
r.levelOrder(r)
