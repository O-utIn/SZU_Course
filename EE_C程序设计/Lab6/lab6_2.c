#include<stdio.h>

float average(int arr[], int n);
void findMax(int arr[], int n);
void searchFail(int arr[], int n);

int main()
{
	int a[10] = {89,78,95,80,70,98,93,50,58,66};
	
	int i;
	printf("ѧ����ź���ѧ�ɼ�����:\n");
	printf("���\t�ɼ�\n");
	for(i = 0;i < 10;i++)
	{
		printf("[%2d]\t%d\n",i + 1,a[i]);
	}
	
	float avg = average(a,10);
	printf("\nƽ���ɼ�Ϊ:%.2f\n",avg);
	
	findMax(a,10);
	
	searchFail(a,10); 
	
	return 0;
}

float average(int arr[], int n)
{
	int i;
	int sum = 0;
	for(i = 0;i < n;i++)
	{
		sum += arr[i];
	}
	return (float)sum / n;
}

void findMax(int arr[], int n)
{
	int max = arr[0];
	int MaxIndex;
	int i;
	for(i = 1;i < n;i++)
	{
		if(arr[i] > max)
		{
			max = arr[i];
			MaxIndex = i + 1;
		}
	}
	printf("��߳ɼ���ѧ�����Ϊ:[%2d],ѧ���ɼ�Ϊ:%d\n",MaxIndex,max);
}

void searchFail(int arr[], int n)
{
	int i;
	int count = 0;
	printf("\n�������ѧ���������:\n");
	printf("���\t�ɼ�\n"); 
	for(i = 0;i < n;i++)
	{
		if(arr[i] < 60)
		{
			printf("[%2d]\t%d\n",i + 1,arr[i]);
			count +=1;
		}
	}
	printf("�������ѧ��������Ϊ:%d",count);
}
