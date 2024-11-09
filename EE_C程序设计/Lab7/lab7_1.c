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
    
    //��ȡ���� 
    printf("������10���������ո�ָ����س�ȷ�ϣ���\n");
    for (i = 0; i < N; i++)
	{
        scanf("%d", &arr[i]);
    }
    
    //���ú����������� 
    separate_sort(arr, N);
    
    //��������� 
    printf("�����Ľ��Ϊ��\n");
    for (i = 0; i < N; i++)
	{
        printf("%d ", arr[i]);
    }
    
    return 0;
}

void sort(int *p, int n, int sort_type) //ʹ��ѡ������ 
{
    int temp;
    int i,j;
    
    //���������������� 
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
    
    //����ż���Ľ����� 
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
    
    //ʹ��ָ�������ż�Ի��� 
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
    
    //���������� 
    sort(a, start - a, odd);
    sort(start, a + n - start, even);
}
