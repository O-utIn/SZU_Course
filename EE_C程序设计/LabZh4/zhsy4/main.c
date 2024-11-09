#include "ball.h"


int main()
{
	struct BALL ball[Num];
	srand((unsigned int)time(NULL));
	
	int n,count;
	int sum = 0;
	
	printf("请输入弹球数量(1-10):");
	scanf("%d",&n);
	
	if(n < 1)
		n=1;
		
	else if(n > Num)
		n = Num;
		
	initBall(ball,n);
	HideCursor();
	
	while(1)
	{
		system("cls");
		count=moveBall(ball,n);
		sum = sum + count;
		redrawCWin(ball,n);
		printf("撞地次数: %d",sum);
		delay(100000);
	}
	
	return 0;
}
