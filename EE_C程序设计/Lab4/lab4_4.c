#include <stdio.h>

int main()
{	
	int year, month, day;
	printf("请输入年份(4位整数):",&year);
	scanf("%d",&year);
	printf("请输入月份(1-12):",&month);
	scanf("%d",&month);
	
	switch(month)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			day = 31;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			day = 30;
			break;
		case 2:
			if((year % 4 == 0 && year % 100 !=0) || year % 400 == 0) 
				day = 29;
			else
				day = 28;
			break;
		default:
            printf("输入的月份无效\n");
            return 0;
	}
	printf("%04d年%02d月有%02d天\n",year,month,day);
	
	return 0;
}
