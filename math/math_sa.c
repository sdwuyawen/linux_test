/**
 * 	@filename :
 * 		math_sa.c
 * 
 * 	@note :
 * 		计算Sa(a)的全时域的积分值
 * 		计算机只能算到有限区段，因为计算机有最大数值限制。
 *		下面用辛普森公式积分。
 *
 **/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double fsimpf(double x) 
{
	if ( fabs(x) < 1e-10) 
		return 1.0;
		
	return sin(x)/x ;
}

double fsimp(double a,double b,double eps) 
{
	int n,k;
	double h,t1,t2,s1,s2,ep,p,x;
	
	n=1; h=b-a;
	t1=h*(fsimpf(a)+fsimpf(b))/2.0; 
	s1=t1;
	ep=eps+1.0;
	
	while (ep>=eps)
	{
		p=0.0;
		for (k=0;k<=n-1;k++)
		{
			x=a+(k+0.5)*h;
			p=p+fsimpf(x);
		}
		
		t2=(t1+h*p)/2.0;
		s2=(4.0*t2-t1)/3.0;
		ep=fabs(s2-s1);
		t1=t2; s1=s2; n=n+n; h=h/2.0;
	}
	
	return(s2);
}

void main()
{
	double a,b,eps,t;
	
	a= -4000.0; b= 4000.0; eps= 1e-10;  
	t=fsimp(a,b,eps);
	printf("%e\n",t);
	printf("\n Press any key to quit...");
} 
