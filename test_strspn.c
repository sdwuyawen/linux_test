/**
 * 	@filename :
 * 		test_strspn.c
 * 
 * 	@note :
 * 	定义函数 size_t strspn (const char *s,const char * accept);
 * 	函数说明 
 * 		strspn()从参数s字符串的开头计算连续的字符，而这些字符都完全是accept所指字符串中的字符。
 * 		简单的说，若strspn()返回的数值为n，则代表字符串s 开头连续有n 个字符都是属于字符串accept内的字符。
 * 	返回值 
 * 		返回字符串s开头连续包含字符串accept内的字符数目。
 * 
 **/
#include <stdio.h>
#include <string.h>
#include <malloc.h>

int main(int argc, char *argv[])
{
	char *input_str ="id=1?dest=2?value=3";
	char *command;
	char *id = NULL;
	char *dest = NULL;
	char *value = NULL;
	int len;
		
	command = strstr(input_str, "id=");
	command += strlen("id=");
	len = strspn(command, "-1234567890");
	id = strndup(command, len);

	command = strstr(input_str, "dest=");
	command += strlen("dest=");
	len = strspn(command, "-1234567890");
	dest = strndup(command, len);

	command = strstr(input_str, "value=");
	command += strlen("value=");
	len = strspn(command, "-1234567890");
	value = strndup(command, len);
	
	printf("%s\n", input_str);
	
	if (id) {
		printf("id = %s\n", id);
		free(id);
	}

	if (dest) {
		printf("dest = %s\n", dest);
		free(dest);
	}
	
	if (value) {
		printf("value = %s\n", value);
		free(value);
	}	
	
	return 0;
}
