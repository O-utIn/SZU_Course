#include <stdio.h>
#include <math.h>
#define rate1 0.0330
#define rate2 0.0275
#define rate3 0.0225
#define rate4 0.0175
#define rate5 0.0030


/*
rate1-rate5对应利率如下：
5年期 定期存款年利率为3.30%；
3年期 定期存款年利率为2.75%；
2年期 定期存款年利率为2.25%；
1年期定期存款年利率为1.75%；
活期存款年利率为0.30%（活期存款每一季度结算一次利息）
*/


int main( )
{
	float principal;
	float income1, income2, income3, income4, income5;

	printf("请输入您的本金（元）："); 
	scanf("%f",&principal);

	income1 = principal + principal * rate1 * 5;
	income2 = (principal + principal * rate3 * 2) * (1.0 + rate2 * 3);
	income3 = (principal + principal * rate2 * 3) * (1.0 + rate3 * 2);
	income4 = principal * pow(1+rate4, 5);
	income5 = principal * pow(1 + rate5 * 0.25, 20);
	
	printf("\n\n存5年期的本息和: %.2f元 \n先存2年再存3年的本息和： %.2f元 \n先存3年再存2年的本息和： %.2f元 \n连存5次1年期的本息和： %.2f元 \n存活期的本息和: %.2f元\n",income1, income2, income3, income4, income5); 

	return 0;
}

