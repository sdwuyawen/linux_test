#!/bin/bash
a="one,two,three,four"
echo "substr = ${a:0:3}"
echo "substr1 = ${a:3}"
echo "char = ${a:0:1} ${a:1:1} ${a:2:1}"

if [ ${a:0:1} = 'o' ]; then
	echo "match OK"
fi

arr=${a//,/ }
for s in ${arr[@]}
do
echo "$s"
done

