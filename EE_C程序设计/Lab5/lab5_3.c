#include<stdio.h>

int main()
{
	int m, n;
	printf("��������������ȵ�������m��n���ո�ָ����룬�س�ȷ��:");
	scanf("%d%d",&m,&n) ;
	
	int lcm, gcd;
	gcd = m > n ? n:m;
	while(gcd > 1 && (m % gcd != 0 || n % gcd !=0))
		gcd--;
	printf("%d��%d�����Լ��Ϊ:%d\n",m,n,gcd); 
	lcm = m * n / gcd;
	printf("%d��%d����С������Ϊ:%d",m,n,lcm);
	return 0;
}
