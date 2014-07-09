#!/bin/sh
echo "======================================================"
find_num=0
while read m_dev m_path m_type m_opt
do
	mymatch=${m_dev#/dev/sd*}
	#echo "match : "$mymatch
	if [ $mymatch != $m_dev ]; then
		echo "dev \t: "$m_dev
		echo "path\t: "$m_path
		echo "type\t: "$m_type
		echo "======================================================"
		find_num=`expr $find_num + 1`
	fi
done < /proc/mounts
echo "find num = "$find_num
if [ $find_num -lt 0 ]; then
	echo "\n"
else
	echo "======================================================\n"
fi
