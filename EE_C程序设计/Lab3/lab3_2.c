#include <stdio.h>
#include <math.h>
#define rate1 0.0330
#define rate2 0.0275
#define rate3 0.0225
#define rate4 0.0175
#define rate5 0.0030


/*
rate1-rate5��Ӧ�������£�
5���� ���ڴ��������Ϊ3.30%��
3���� ���ڴ��������Ϊ2.75%��
2���� ���ڴ��������Ϊ2.25%��
1���ڶ��ڴ��������Ϊ1.75%��
���ڴ��������Ϊ0.30%�����ڴ��ÿһ���Ƚ���һ����Ϣ��
*/


int main( )
{
	float principal;
	float income1, income2, income3, income4, income5;

	printf("���������ı���Ԫ����"); 
	scanf("%f",&principal);

	income1 = principal + principal * rate1 * 5;
	income2 = (principal + principal * rate3 * 2) * (1.0 + rate2 * 3);
	income3 = (principal + principal * rate2 * 3) * (1.0 + rate3 * 2);
	income4 = principal * pow(1+rate4, 5);
	income5 = principal * pow(1 + rate5 * 0.25, 20);
	
	printf("\n\n��5���ڵı�Ϣ��: %.2fԪ \n�ȴ�2���ٴ�3��ı�Ϣ�ͣ� %.2fԪ \n�ȴ�3���ٴ�2��ı�Ϣ�ͣ� %.2fԪ \n����5��1���ڵı�Ϣ�ͣ� %.2fԪ \n����ڵı�Ϣ��: %.2fԪ\n",income1, income2, income3, income4, income5); 

	return 0;
}

