/**
 * 	@filename :
 * 		test_root.c
 * 
 * 	@note :
 * 		用牛顿迭代法求方程的根。方程为aX^3+bX^2+cX+d=0, 系数由用户输入, 求X在1附近的根
 *		迭代公式：X(n+1) = Xn - f(Xn)/d(f(Xn))
 *
 *
 **/
#include <stdio.h>
#include <math.h>

float root(float a, float b, float c, float d)
{
	float x0, x1, f, f1;
	
	printf("a,b,c,d = %f,%f,%f,%f\n", a, b, c, d);
	x1 = 1;	
	do {
		x0 = x1;
		f = a*x0*x0*x0 + b*x0*x0 + c*x0 + d;
		f1 = 3*a*x0 + 2*b*x0 + c;
		x1 = x0 - f/f1;
		printf("x1 = %f\n", x1);
	} while (fabs(x1-x0) > (1e-5));
	
	printf("x1 = %f\n", x1);
	
	return (x1);
}

float check_x(float a, float b, float c, float d, float x0)
{
	return a*x0*x0*x0 + b*x0*x0 + c*x0 + d;
}

void main()
{
	float a, b, c, d;
	float x;
	
	printf("please input a,b,c,d\n");
	scanf("%f,%f,%f,%f", &a, &b, &c, &d);
	x = root(a, b, c, d);
	printf("x = %10.5f\n", x);
	if (!__isnan(x) && !__isinf(x)) {
		printf("check = %f\n", check_x(a, b, c, d, x));
	}
}
