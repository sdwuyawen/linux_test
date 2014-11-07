/**
 * @filename :
 * 		test_getopt.c
 * @note :
 * 		./bin/test_getopt -a ima -b host -ckeke -d haha
 **/
  
/*
  (1).
	#include <unistd.h>
	
	extern char *optarg;  //选项的参数指针
	extern int optind,    //下一次调用getopt的时，从optind存储的位置处重新开始检查选项。 
	extern int opterr,    //当opterr=0时，getopt不向stderr输出错误信息。
	extern int optopt;    //当命令行选项字符不包括在optstring中或者选项缺少必要的参数时，该选项存储在optopt中，getopt返回'？’、
	
	int getopt(int argc, char * const argv[], const char *optstring);
		调用一次，返回一个选项。 在命令行选项参数再也检查不到optstring中包含的选项时，返回－1，同时optind储存第一个不包含选项的命令行参数。
	首先说一下什么是选项，什么是参数。

	字符串optstring可以下列元素，
		1.单个字符，表示选项，
		2.单个字符后接一个冒号：表示该选项后必须跟一个参数。参数紧跟在选项后或者以空格隔开。该参数的指针赋给optarg。
		3 单个字符后跟两个冒号，表示该选项后必须跟一个参数。参数必须紧跟在选项后不能以空格隔开。该参数的指针赋给optarg。（这个特性是GNU的扩张）	
  (2).
	getopt_long支持长选项的命令行解析，使用man getopt_long，得到其声明如下：
		#include <getopt.h>

		int getopt_long(int argc, char * const argv[],
                  const char *optstring,
                  const struct option *longopts, int *longindex);

		int getopt_long_only(int argc, char * const argv[],
                  const char *optstring,
                  const struct option *longopts, int *longindex);

	说明：函数中的argc和argv通常直接从main()到两个参数传递而来。optsting是选项参数组成的字符串，如
	果该字符串里任一字母后有冒号，那么这个选项就要求有参数。下一个参数是指向数组的指针，这个数组是
	option结构数组，option结构称为长选项表，其声明如下：
		struct option {
              const char *name;
              int has_arg;
              int *flag;
              int val;
          };
		结构中的元素解释如下：
		const char *name：选项名，前面没有短横线。譬如"help"、"verbose"之类。
		
		int has_arg：描述长选项是否有选项参数，如果有，是哪种类型的参数，其值见下表:
				符号常量             数值            含义
			no_argument            0            选项没有参数
			required_argument      1            选项需要参数
			optional_argument      2            选项参数是可选的
			 
		int *flag：
			如果该指针为NULL，那么getopt_long返回val字段的值；
			如果该指针不为NULL，那么会使得它所指向的结构填入val字段的值，同时getopt_long返回0
			 
		int val：
			如果flag是NULL，那么val通常是个字符常量，如果短选项和长选项一致，那么该字符就应该与optstring中出现的这个选项的参数相同；
	
	最后一个参数：longindex参数一般赋为NULL即可；如果没有设置为NULL，那么它就指向一个变量，这个变量会被赋值为寻找到的长选项在longopts中的索引值，这可以用于错误诊断。
	注：GNU提供的getopt_long()和getopt_long_only()函数，其中，后者的长选项字串是以一个短横线开始的，而非一对短横线。
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int result;
	
	opterr = 0;  //使getopt不行stderr输出错误信息
	
	while ((result = getopt(argc, argv, "ab:c::")) != -1) {
		switch(result)	{
			case 'a':
				printf("option=a, optopt=%c, optarg=%s\n", optopt, optarg);
				break;
			case 'b':
				printf("option=b, optopt=%c, optarg=%s\n", optopt, optarg);
				break;
			case 'c':
				printf("option=c, optopt=%c, optarg=%s\n", optopt, optarg);
				break;
			case '?':
				printf("result=?, optopt=%c, optarg=%s\n", optopt, optarg);
				break;
			default:
				printf("default, result=%c\n",result);
				break;
		}
		printf("argv[%d]=%s\n", optind, argv[optind]);
	}
	printf("result=-1, optind=%d\n", optind);   //看看最后optind的位置
	
	for(result = optind; result < argc; result++) {
		printf("-----argv[%d]=%s\n", result, argv[result]);
	}
	
	//看看最后的命令行参数，看顺序是否改变了哈。
	for(result = 1; result < argc; result++) {		
		printf("\nat the end-----argv[%d]=%s\n", result, argv[result]);
	}
		
	return 0;
}
