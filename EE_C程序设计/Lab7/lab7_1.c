#include <stdio.h>

#define N 10
#define odd 1
#define even 2

void sort(int *p, int n, int sort_type);
void separate_sort(int a[], int n);

int main()
{
    int arr[N];
    int i,j;
    
    //获取输入 
    printf("请输入10个整数（空格分隔，回车确认）：\n");
    for (i = 0; i < N; i++)
	{
        scanf("%d", &arr[i]);
    }
    
    //调用函数进行排序 
    separate_sort(arr, N);
    
    //输出排序结果 
    printf("排序后的结果为：\n");
    for (i = 0; i < N; i++)
	{
        printf("%d ", arr[i]);
    }
    
    return 0;
}

void sort(int *p, int n, int sort_type) //使用选择排序 
{
    int temp;
    int i,j;
    
    //对于奇数的升序处理 
    if (sort_type == odd)
	{
        for (i = 0; i < n - 1; i++)
		{
            for (j = 0; j < n - 1 - i; j++)
			{
                if (p[j] > p[j + 1])
				{
                    temp = p[j];
                    p[j] = p[j + 1];
                    p[j + 1] = temp;
                }
            }
        }
    }
    
    //对于偶数的降序处理 
	else if (sort_type == even)
	{
        for (i = 0; i < n - 1; i++)
		{
            for (j = 0; j < n - 1 - i; j++)
			{
                if (p[j] < p[j + 1])
				{
                    temp = p[j];
                    p[j] = p[j + 1];
                    p[j + 1] = temp;
                }
            }
        }
    }
}

void separate_sort(int a[], int n)
{
    int *start = a;
    int *end = a + n - 1;
    
    //使用指针进行奇偶性划分 
    while (start < end)
	{
        while (*start % 2 != 0)
		{
            start++;
        }
        while (*end % 2 == 0)
		{
            end--;
        }
        
        if (start < end)
		{
            int temp = *start;
            *start = *end;
            *end = temp;
        }
    }
    
    //调用排序函数 
    sort(a, start - a, odd);
    sort(start, a + n - start, even);
}
