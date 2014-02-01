/**
 * 	@filename :
 * 		test_fibonacci.c
 * 
 * 	@note :
 * 		
 * 
 **/
/*  
斐波那契数列的性质
	1. 前面相邻两项之和，构成了后一项
	2. 斐波那契数列中任一项的平方数都等于跟它相邻的前后两项的乘积加1或减1;
	3. 任取相邻的四个斐波那契数，中间两数之积（内积）与两边两数之积（外积）相差1.

同样我们还可以有t阶斐波那契数列，通过递推数列a(n+t)=a(n+t-1）+a(n+t-2）+...+a(n），其中a⑴=a⑵=1，以及对于3-t<=n<=0，有a(n)=0.
给出了t阶斐波那契数列的通项公式：
	[r^(n-1）（r-1）/((t+1）r-2t)]，其中r是方程x^{t+1}-2x^t+1=0的唯一一个大于1的正数根（可以看出r非常接近2）
*/
#include "stdio.h"

#define Max 10000 //最长数据长度除以四，开10000时理论上单个数应该能输到40000位的长度。

//a, b分别存储相邻的两个数, 并不是序列
int a[Max],b[Max],n;

void add(int c[], int d[])
{
	int i;
	
	for (i = 0; i < Max; i++) {
		c[i] += d[i];
	}
	
	for (i = 0; i < Max; i++) {
		if(c[i] >= 10000) {
			c[i+1] += c[i]/10000;
			c[i] %= 10000;
		}
	}
}

void print(int c[])
{
	int i;
	int k = 0;
	
	for (i = Max-1; i >= 0; i--) {
		if (k) {
			printf("%d%d%d%d",c[i]/1000,c[i]%1000/100,c[i]%100/10,c[i]%10);
		} else if (c[i] != 0) {
			printf("%d", c[i]);
			k = 1;
		}
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	int i;

	if (argc != 2) {
		return -1;
	}
	
	n = atoi(argv[1]);
	a[0]=1;
	b[0]=1;
	printf("1\n1\n");
	for (i = 1; i < n-1; i++) {
		if (i & 0x01) {
			add(a,b);
			print(a);
		} else {
			add(b,a);
			print(b);
		}
	}
	
	//getchar();
	//getchar();
	
	return 0;
}
