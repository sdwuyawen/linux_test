#!/bin/sh
#
#FILO的stack例子
#
push_func()
{
   #注意如果没有给出func的参数，则在func中跳出，而不是终结整个脚本
	entry=${1:?"Please enter the entry as push_func param"}
	#注意在bottom后面有一个空格，我们使用空格作为分割，这是为了pop最后一个entry用
	mystack="$entry ${mystack:-bottom }"
	echo "push $entry in stack : $mystack"
}

pop_func()
{
	#删除最top的entry
	mystack=${mystack#*' '}
   #获取当前stack的top元素 。
	entry=${mystack%% *}
	echo "After pop, top entry is '$entry', stack is $mystack"
}

push_func one
push_func two
pop_func
pop_func
pop_func
