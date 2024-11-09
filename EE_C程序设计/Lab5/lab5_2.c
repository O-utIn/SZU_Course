#include<stdio.h>
#include<math.h> 

int main()
{
	double x;
	
	printf("请输入x的角度值(弧度制)以计算sin(x)的值:");
	scanf("%lf",&x);
	
	double term = x;
	double sinX = term;
	int n = 1;
	double threshold = 1e-5;
	
	while(fabs(term) >= threshold)
	{
		term = -term * x * x / ((2 * n)  * (2 * n + 1));
		sinX += term;
		n++;
	}
	printf("sin(%lf) = %lf\n", x, sinX);
	printf("此时的累加项数为:%d",n);
	return 0;
}
