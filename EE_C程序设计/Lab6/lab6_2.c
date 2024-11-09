#include<stdio.h>

float average(int arr[], int n);
void findMax(int arr[], int n);
void searchFail(int arr[], int n);

int main()
{
	int a[10] = {89,78,95,80,70,98,93,50,58,66};
	
	int i;
	printf("学生序号和数学成绩如下:\n");
	printf("序号\t成绩\n");
	for(i = 0;i < 10;i++)
	{
		printf("[%2d]\t%d\n",i + 1,a[i]);
	}
	
	float avg = average(a,10);
	printf("\n平均成绩为:%.2f\n",avg);
	
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
	printf("最高成绩的学生序号为:[%2d],学生成绩为:%d\n",MaxIndex,max);
}

void searchFail(int arr[], int n)
{
	int i;
	int count = 0;
	printf("\n不及格的学生情况如下:\n");
	printf("序号\t成绩\n"); 
	for(i = 0;i < n;i++)
	{
		if(arr[i] < 60)
		{
			printf("[%2d]\t%d\n",i + 1,arr[i]);
			count +=1;
		}
	}
	printf("不及格的学生总人数为:%d",count);
}
