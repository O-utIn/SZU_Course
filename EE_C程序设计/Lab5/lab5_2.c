#include<stdio.h>
#include<math.h> 

int main()
{
	double x;
	
	printf("������x�ĽǶ�ֵ(������)�Լ���sin(x)��ֵ:");
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
	printf("��ʱ���ۼ�����Ϊ:%d",n);
	return 0;
}
