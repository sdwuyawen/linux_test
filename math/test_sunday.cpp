/*
 * test_sunday.cpp
 * 
 * Copyright 2014 paul <paul@paul-desktop>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

/*
Sunday-字符串匹配算法 -- 一种优于 KMP 的算法
思想类似于BM 算法，只不过是从左向右匹配
遇到不匹配的看大串中匹配范围之外的右侧第一个字符在小串中的最右位置
另外：采用BM/KMP 的预处理的做法，事先计算好移动步长 ，等到遇到不匹配的值直接使用
*/
#include <iostream>
#include <string.h>

using namespace std;

//一个字符8位 最大256种
#define MAX_CHAR_SIZE 256

#define MATH_DEBUG

/*
设定每个字符最右移动步长，保存每个字符的移动步长
如果大串中匹配字符的右侧一个字符没在子串中，大串移动步长= 整个串的距离 +1
如果大串中匹配范围内的右侧一个字符在子串中，大串移动距离= 子串长度 - 这个字符在子串中的位置
*/
int *setCharStep(char *subStr)
{
	int *charStep = new int[MAX_CHAR_SIZE];
	int subStrLen = strlen(subStr);
	
	for (int i=0; i<MAX_CHAR_SIZE; i++) {
		charStep[i] = subStrLen+1;
	}
	
	//从左向右扫描一遍 保存子串中每个字符所需移动步长
	for (int i=0; i<subStrLen; i++) {
		charStep[(unsigned char)subStr[i]] = subStrLen - i;         
	}
	
	return charStep;
}

/*
算法核心思想，从左向右匹配，遇到不匹配的看大串中匹配范围之外的右侧第一个字符在小串中的最右位置
根据事先计算好的移动步长移动大串指针，直到匹配
*/
int sundaySearch(char *mainStr,char *subStr,int *charStep)
{
	int mainStrLen = strlen(mainStr);
	int subStrLen = strlen(subStr);
	int main_i = 0;
	int sub_j = 0;
	
#ifdef MATH_DEBUG
	cout<<"main str "<<"("<<mainStrLen<<") :"<<endl;
	cout<<"  "<<mainStr<<endl;
	
	cout<<"sub str "<<"("<<subStrLen<<") :"<<endl;
	cout<<"  "<<subStr<<endl;
#endif
	
	while (main_i < mainStrLen) {                  
		//保存大串每次开始匹配的起始位置，便于移动指针
		int temp = main_i;
		
		while (sub_j < subStrLen) {
			if (mainStr[main_i] == subStr[sub_j]) {
				main_i++;
				sub_j++;
				continue;                   
			} else {
				//如果匹配范围外已经找不到右侧第一个字符，则匹配失败
				if (temp+subStrLen > mainStrLen) {
					return -1;
				}
				
				//否则 移动步长 重新匹配
				char firstRightChar = mainStr[temp + subStrLen];
				main_i = temp + charStep[(unsigned char)firstRightChar];
				sub_j = 0;   
				break;//退出本次失败匹配 重新一轮匹配
			}  
		}
		
		if (sub_j == subStrLen) {
			return main_i - subStrLen;
		}
	}
	
	return -1;
}

int main()
{
	 char *mainStr = (char *)"absaddsasfasdfasdf";
	 char *subStr = (char *)"dd";

	 int *charStep = setCharStep(subStr);
	 cout<<"========================================"<<endl;
	 int pos = sundaySearch(mainStr, subStr, charStep);
	 cout<<"========================================"<<endl;
	 cout<<"position："<<pos<<endl;
	 cout<<"========================================"<<endl;

	 return 0;    
} 
