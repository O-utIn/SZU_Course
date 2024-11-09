#include <stdio.h>
#define PI 3.14159265
int main( )
{
	float radius, high;
	float bottom_area, surface_area, volume;

	printf("请输入圆柱体的底面半径和高，以空格隔开："); 
	scanf("%f %f",&radius,&high);

	bottom_area = radius * radius * PI;
	surface_area = 2 * radius * radius * PI + 2 * radius * PI * high;
	volume = radius * radius * PI * high;

	printf("\n\n圆柱的半径= %f \n圆柱的高 = %f \n圆柱的底面积 = %f \n圆柱的表面积 = %f\n 圆柱的体积 = %f\n",radius, high, bottom_area, surface_area, volume); 
	printf("\n\n圆柱的半径= %5.2f \n圆柱的高 = %5.2f \n圆柱的底面积 = %5.2f \n圆柱的表面积 = %5.2f \n圆柱的体积 = %5.2f\n",radius, high, bottom_area, surface_area, volume); 
	printf("\n\n圆柱的半径= %g \n圆柱的高 = %g \n圆柱的底面积 = %g \n圆柱的表面积 = %g \n圆柱的体积 = %g\n",radius, high, bottom_area, surface_area, volume); 

	return 0;
}

