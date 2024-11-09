#include<stdio.h>

int main()
{
	int m, n;
	printf("请输入两个不相等的正整数m、n，空格分隔输入，回车确认:");
	scanf("%d%d",&m,&n) ;
	
	int lcm, gcd;
	gcd = m > n ? n:m;
	while(gcd > 1 && (m % gcd != 0 || n % gcd !=0))
		gcd--;
	printf("%d和%d的最大公约数为:%d\n",m,n,gcd); 
	lcm = m * n / gcd;
	printf("%d和%d的最小公倍数为:%d",m,n,lcm);
	return 0;
}
