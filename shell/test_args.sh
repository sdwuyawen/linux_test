#!/bin/sh
echo "arg string : $@"
echo "arg num    : $#"

##
SRC_W=1024
SRC_H=768
DST_W=1024
DST_H=600

let DST_X=$SRC_W-$DST_W
let DST_Y=($SRC_H-$DST_H)/2

echo "math0: $DST_X $DST_Y"

DST_X=$(( 2 + 1 ))
DST_Y=$[ 2 + 1]

echo "math1: $DST_X ${DST_Y}"

r0=`expr 4 + 5`
r1=`expr 4+5`
echo "math2: $r0 <> $r1"

#注：expr 沒有乘幂
#乘幂 (如 2 的 3 次方)
r=$(( 2 ** 3 ))
echo "math3-0: $r"
r=$[ 2 ** 3 ]
echo "math3-1: $r"

#乘冪：
m=2
let r=m**2
echo "math4: $r"

echo "dir=$(dirname $0)"
echo "name=$(basename $0)"

#下面的语句在当前的shell环境展开并执行test_args1.sh，可以改变当前的环境变量，在test_args1.sh的文件头可以不写#!/bin/sh
. $(dirname $0)/test_args1.sh
#下面的语句执行test_args1.sh, 不能可以改变当前的环境变量
#$(dirname $0)/test_args1.sh

num=$#
while [ $# -gt 0 ]; do
	echo "[${num}-$#] : $1";
	test_printf $1 $1
	shift;
done 
