#include<stdio.h>
#include<math.h>

int main()
{
	printf("300以内的所有素数为:\n"); 
	int flag,n=0;
	int i,j;
	for(i = 2;i <= 300;i++)
	{
		flag = 1;

		for(j=2 ;j < i;j++)
		{

			if(i % j == 0)
				flag = 0;

		}
		if(flag == 1)
		{
			printf("%-8d",i);
			n += 1;
			if(n % 8 == 0)
				printf("\n");
		}
	}
	return 0;
}
