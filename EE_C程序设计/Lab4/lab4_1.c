#include <stdio.h>

int main()
{
	float weight, height, t;
	printf("请输入您的体重（kg）、身高(m)，空格分隔数据，回车确认:");
	scanf("%f%f",&weight, &height);
	t = weight / height / height;
	printf("体脂数为%.1f",t);
	if(t < 18)
		printf("为低体重");
	else if(18 <= t <= 25)
		printf("为正常体重");
	else if(25 <= t <= 27)
		printf("为超重体重");
	else
		printf("为肥胖"); 
	return 0;
}
