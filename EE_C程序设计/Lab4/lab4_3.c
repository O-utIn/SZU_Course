#include <stdio.h>
#include <stdlib.h>

int main()
{
	srand((unsigned int)time(NULL));
	int a = rand() % 100;
	int b = rand() % 100;
	int temp;
	printf("�����������Ϊ��%d %d\n",a,b);
	if(a>=b)
		;
	else
	{
		temp = a;
		a = b;
		b = temp;
	}
	printf("����������Ϊ��%d %d\n",a,b);
	return 0;
}
