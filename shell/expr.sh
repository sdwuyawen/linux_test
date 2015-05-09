#!/bin/sh
#expr返回11(匹配字节数), $?=0
chitype=hi3716mv101
#expr返回0, $?=1
#chitype=hi3716hv101
expr $chitype : ".*m.*"
echo $?

string="hello,everyone my name is xiaoming"
echo "=========="
#->34
echo ${#string}
echo $?
echo "=========="
#->0
expr index "$string" everyone
echo $?
echo "=========="
#->34
expr "$string" : ".*my.*"
echo $?
