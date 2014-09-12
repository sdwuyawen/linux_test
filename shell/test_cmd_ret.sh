#!/bin/bash
#
#测试 &&, ||符号的使用
#判断前面命令的输出是否有
#	&& 前面输出正常(返回0)会执行下&&后面的命令
#   || 前面输出正常(返回0)会执行下一行命令，不执行||后面的命令；不正常，则执行后面的命令
#

do_cmd1() {
	echo "hello1"
	return 0
}

function do_cmd2()
{
	echo "hello2"
	return 1
}

#如果没有指定返回值，默认返回0
function do_cmd3() 
{
	echo "hello3"
	return
}

strtok() { # <string> { <variable> [<separator>] ... }
	local tmp
	local val="$1"
	local count=0

	shift

	while [ $# -gt 1 ]; do
		tmp="${val%%$2*}"

		[ "$tmp" = "$val" ] && break

		val="${val#$tmp$2}"

		export ${NO_EXPORT:+-n} "$1=$tmp"; count=$((count+1))
		shift 2
	done

	if [ $# -gt 0 -a -n "$val" ]; then
		export ${NO_EXPORT:+-n} "$1=$val"; count=$((count+1))
	fi

	return $count
}

cmd1_ret=`do_cmd1`
#显示hello1
echo "cmd1_ret = $cmd1_ret"
is_cmd1_ok=`do_cmd1 > /dev/null && echo OK || echo FAIL`
#因为return 0, 显示OK； 非0, 没有显示OK, 而是显示FAIL
echo "$is_cmd1_ok"

cmd2_ret=`do_cmd2`
#显示hello2
echo "cmd2_ret = $cmd2_ret"
is_cmd2_ok=`do_cmd2 > /dev/null && echo OK || echo FAIL`
#非0, 没有显示OK, 而是显示FAIL
echo "$is_cmd2_ok"

cmd3_ret=`do_cmd3`
#显示hello3
echo "cmd3_ret = $cmd3_ret"
is_cmd3_ok=`do_cmd3 > /dev/null && echo OK || echo FAIL`
echo "$is_cmd3_ok"

#
strtok "hello world" str1 " "
#return 1
echo $?
echo ${str1}
#
strtok "hello world" str1 " " str2 " "
#return 2
echo $?
echo ${str1}
echo ${str2}
