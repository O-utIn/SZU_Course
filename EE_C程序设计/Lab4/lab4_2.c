#include <stdio.h>

int main()
{
	int basic, extra;
	printf("请输入综合基本成绩和附加题成绩(均为整数)，空格分隔数据，回车确认:");
	scanf("%d%d",&basic, &extra);
	if(basic > 100 || basic < 0 || extra > 30 || extra <0)
	{
		printf("成绩不合法");
		return 0;
	}
	else
		printf("综合基础成绩为:%d\n附加题成绩为:%d\n",basic,extra);
		if(basic < 60)
			printf("等级为F,对应绩点为0.0");
		else if(60 <= basic &&basic <= 64)
			printf("等级为D,对应绩点为1.0");
		else if(65 <= basic && basic <= 69)
			printf("等级为C,对应绩点为2.0");
		else if(70 <= basic && basic <= 74)
			printf("等级为C+,对应绩点为2.5");
		else if(75 <= basic && basic <= 79)
			printf("等级为B,对应绩点为3.0");
		else if(80 <= basic && basic <= 84) 
		{
			if(18 <= extra && extra <= 23)
				printf("等级为A,对应绩点为4.0");
			else if(24 <= extra && extra <=30)
				printf("等级为A+，对应绩点为4.5");
			else
				printf("等级为B+,对应绩点为3.5");
		}
		else if(85 <= basic && basic <=92)
		{
			if(18 <= extra && extra <=30)
				printf("等级为A+，对应绩点为4.5");
			else
				printf("等级为A，对应绩点为4.0");
		}
		else
			printf("等级为A+，对应绩点为4.5");

	return 0;
}
