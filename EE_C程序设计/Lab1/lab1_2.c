// ������ : ����һ������Բ��������ܳ��ĳ���,ע��������ʱ����������
// ����: �뾶
// ���:Բ��������ܳ����������� 4 �������ʽ
#include <stdio.h>
#define PI 3.14159265

int main( )
{
	float radius;
	float area, perimeter;

	printf("������Բ�İ뾶��");
	scanf("%f",&radius);

	area = radius * radius * PI;
	perimeter = 2 * radius * PI;

	printf("\n\nԲ�İ뾶= %f \nԲ����� = %f \nԲ���ܳ� = %f\n",radius, area, perimeter);
	printf("\n\nԲ�İ뾶= %5.2f \nԲ����� = %5.2f \nԲ���ܳ� = %5.2f\n",radius, area, perimeter);
	printf("\n\nԲ�İ뾶= %d \nԲ����� = %d \nԲ���ܳ� = %d\n",radius, area, perimeter);
	printf("\n\nԲ�İ뾶= %d \nԲ����� = %d \nԲ���ܳ� = %d\n",(int)radius, (int)area, (int)perimeter);
	printf("\n\n");

	return 0;
}

