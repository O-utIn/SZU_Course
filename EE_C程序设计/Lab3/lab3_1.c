#include <stdio.h>
#define PI 3.14159265
int main( )
{
	float radius, high;
	float bottom_area, surface_area, volume;

	printf("������Բ����ĵ���뾶�͸ߣ��Կո������"); 
	scanf("%f %f",&radius,&high);

	bottom_area = radius * radius * PI;
	surface_area = 2 * radius * radius * PI + 2 * radius * PI * high;
	volume = radius * radius * PI * high;

	printf("\n\nԲ���İ뾶= %f \nԲ���ĸ� = %f \nԲ���ĵ���� = %f \nԲ���ı���� = %f\n Բ������� = %f\n",radius, high, bottom_area, surface_area, volume); 
	printf("\n\nԲ���İ뾶= %5.2f \nԲ���ĸ� = %5.2f \nԲ���ĵ���� = %5.2f \nԲ���ı���� = %5.2f \nԲ������� = %5.2f\n",radius, high, bottom_area, surface_area, volume); 
	printf("\n\nԲ���İ뾶= %g \nԲ���ĸ� = %g \nԲ���ĵ���� = %g \nԲ���ı���� = %g \nԲ������� = %g\n",radius, high, bottom_area, surface_area, volume); 

	return 0;
}

