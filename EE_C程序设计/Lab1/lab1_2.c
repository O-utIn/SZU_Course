// 程序功能 : 这是一个计算圆的面积和周长的程序,注意程序输出时的数据类型
// 输入: 半径
// 输出:圆的面积和周长，程序中有 4 种输出形式
#include <stdio.h>
#define PI 3.14159265

int main( )
{
	float radius;
	float area, perimeter;

	printf("请输入圆的半径：");
	scanf("%f",&radius);

	area = radius * radius * PI;
	perimeter = 2 * radius * PI;

	printf("\n\n圆的半径= %f \n圆的面积 = %f \n圆的周长 = %f\n",radius, area, perimeter);
	printf("\n\n圆的半径= %5.2f \n圆的面积 = %5.2f \n圆的周长 = %5.2f\n",radius, area, perimeter);
	printf("\n\n圆的半径= %d \n圆的面积 = %d \n圆的周长 = %d\n",radius, area, perimeter);
	printf("\n\n圆的半径= %d \n圆的面积 = %d \n圆的周长 = %d\n",(int)radius, (int)area, (int)perimeter);
	printf("\n\n");

	return 0;
}

