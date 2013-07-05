#!/bin/sh
# ”#“开始表示注释行，一个良好的变成习惯是文件的开始给出文件的信息，以便阅读
# highest [howmany]
# sort the entry inf on a descend order, and high line the top
# [hwomany] entry, default is 5
#
# 1 -> 要求用户给出参数<文件名>，否则退出脚本，2 -> 要求用户给出显示的条目，否则使用缺省值5
filename=${1?'Used command ./test [entry_num]'}
howmany=${2:-5}
# header是用来表明输出的时候显示的某些信息，如果这行我们注释掉，将不显示
header=set
echo "howmany = $howmany"
#echo带有两个参数-n，表示显示后不执行LINEFEED，即不换行。-e表示解析\n，不将其作为两个字符，视为换行符号。我们如果注释掉header，则不会显示任何内容。
echo -e -n ${header:+"num    name\n"}
echo -e -n ${header:+"----    -----\n"}
#sort是个排序的命令，-r表示reverse的顺序，即从大到小，-n表示将第一个参数视为数字，而不是character的方式。| 表示pipe输出，head -N 表示显示头N行。
sort -nr $filename | head -$howmany
