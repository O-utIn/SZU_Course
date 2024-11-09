#include<stdio.h>
#define N 10

void generate(int a[][N], int n);
void printSimpleYanghui(int a[][N],int n);
void printRegularYanghui(int a[][N],int n);

int main()
{
	int arr[N][N];
	int n;
	
	printf("请输入杨辉三角形的行数:");
	scanf("%d",&n);
	
	generate(arr,n);
	printSimpleYanghui(arr,n);
	printRegularYanghui(arr,n);
	
	return 0;
}

void generate(int a[][N], int n)
{
	int i,j;
	for(i = 0;i < n;i++)
	{
		a[i][0] = 1;
		a[i][i] = 1;
	}
	for(i = 2;i< n;i++)
	{
		for(j = 1;j < i;j++)
		{
			a[i][j] = a[i - 1][j - 1] + a[i - 1][j];
		}
	}
}

void printSimpleYanghui(int a[][N],int n)
{
	int i,j;
	printf("\n直角杨辉三角形:\n");
	for(i = 0;i < n;i++)
	{
		for(j = 0;j <= i;j++)
		{
			printf("%4d",a[i][j]);
		}
		printf("\n");
	}
}

void printRegularYanghui(int a[][N],int n)
{
	int i,j;
	printf("\n等腰杨辉三角形:\n");
	for(i = 0;i < n;i++)
	{
		for(j = 0;j < n - i - 1;j++)
		{
			printf("  ");
		}
		for(j = 0;j <= i;j++)
		{
			printf("%4d",a[i][j]);
		}
		printf("\n");
	}
}
