#include <stdio.h>

int main()
{
	float weight, height, t;
	printf("�������������أ�kg�������(m)���ո�ָ����ݣ��س�ȷ��:");
	scanf("%f%f",&weight, &height);
	t = weight / height / height;
	printf("��֬��Ϊ%.1f",t);
	if(t < 18)
		printf("Ϊ������");
	else if(18 <= t <= 25)
		printf("Ϊ��������");
	else if(25 <= t <= 27)
		printf("Ϊ��������");
	else
		printf("Ϊ����"); 
	return 0;
}
