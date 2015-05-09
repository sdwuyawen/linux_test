1. 语法：eval cmdLine
	eval会对后面的cmdLine进行两遍扫描，如果第一遍扫描后，cmdLine是个普通命令，则执行此命令；如果cmdLine中含有变量的间接引用，则保证间接引用的语义。
	
	举例如下：
	set 11 22 33 44

	如果要输出最近一个参数，即44，可以使用如下命令，
	echo $4

	但是如果我们不知道有几个参数的时候，要输出最后一个参数，大家可能会想到使用$#来输出最后一个参数，
	如果使用命令：

	echo "\$$#"
	则得到的结果是 $4，而不是我们想要的44。这里涉及到一个变量间接引用的问题，我们的本意是输出 $4，默认情况下，命令后忽略变量间接引用的情况。

	这时候，就可以使用eval命令。
	eval echo "\$$#"
	得到的结果为44	
	
2. ${1:-}
	$1是函数的第一个参数;
	替换的定义，${varname:-word}，如果varname存在且非null，则返回其值；否则，返回word。
	用途：如果变量未定义，则返回默认值。
	${1:-}的意思就是说，如果函数有第一个参数，就返回这个参数，如果没有，就返回空。

	${varname:+word}
	当$varname已经设置时进行替换为word，否则，没有设置或为空，不进行替换。

	又如 export ${NO_EXPORT:+-n}
	如果NO_EXPORT定义了，则使用export -n
	如果NO_EXPORT没定义，则使用export	
	
	
3.
[ -d DIR ] 				如果DIR存在并且是一个目录则为真
[ -f FILE ] 			如果FILE存在且是一个普通文件则为真
[ -s file ]     		判断文件的大小是否非0. 如果文件存在并且大小非0，则返回true
[ -z STRING ] 			如果STRING的长度为零则为真
[ -n STRING ] 			如果STRING的长度非零则为真
[ STRING1 = STRING2 ] 	如果两个字符串相同则为真
[ STRING1 != STRING2 ] 	如果字符串不相同则为真
[ ARG1 OP ARG2 ] 		ARG1和ARG2应该是整数或者取值为整数的变量，OP是-eq（等于）-ne（不等于）-lt（小于）-le（小于等于）-gt（大于）-ge（大于等于）之中的一个

4. set
   set [--abefhkmnptuvxBCEHPT] [-o option] [arg ...]
   set [+abefhkmnptuvxBCEHPT] [+o option] [arg ...]
   set -e表示一旦脚本中有命令的返回值为非0，则脚本立即退出，后续命令不再执行;
   set -o pipefail表示在管道连接的命令序列中，只要有任何一个命令返回非0值，则整个管道返回非0值，即使最后一个命令返回0.
   
   范例: ./testset.sh
   输出：
disable exit on non-zero return status and pipefail track
./testset.sh: 行 6: 1/0: 除0 （错误符号是 "0"）
return status = 0
disable exit on non-zero return status but enable pipefail track
./testset.sh: 行 12: 1/0: 除0 （错误符号是 "0"）
return status = 1
enable exit on non-zero return status and pipefail track
./testset.sh: 行 18: 1/0: 除0 （错误符号是 "0"）
   
5. expr用法
  string="hello,everyone my name is xiaoming"
  1).计算字符串的长度
	我们可以用awk中的length(s)进行计算。我们也可以用echo中的echo ${#string}进行计算，当然也可以expr中的expr length $string 求出字符串的长度.
	expr length "$string"  
	-> 34
	
  2). expr中的expr index $string substring索引命令功能在字符串$string上找出substring中的字符第一次出现的位置，若找不到则expr index返回0或1
	expr index "$string" my
    -> 11
    y出现在string的11(从1开始)
    
    expr index "$string" nihao
    -> 1
    h出现在string的1
    
  3). expr中的expr match $string substring命令在string字符串中匹配substring字符串，然后返回匹配到的substring字符串的长度，若找不到则返回0。
	expr match "$string" my
	-> 0
	expr match "$string" hell.*
	-> 34	
	expr "$string" : ".*my.*"
	->
	
	substring从string的开始位置匹配才行.
	
  4). 在shell中可以用{string:position}和{string:position:length}进行对string字符串中字符的抽取。
	第一种是从position位置开始抽取直到字符串结束，
	第二种是从position位置开始抽取长度为length的子串。而用expr中的expr substr $string $position $length同样能实现上述功能。
	
	echo ${string:10}
	echo ${string:10:5}
	echo ${string:10:10}
	expr substr "$string" 10 5
