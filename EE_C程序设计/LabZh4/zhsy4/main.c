#include "ball.h"


int main()
{
	struct BALL ball[Num];
	srand((unsigned int)time(NULL));
	
	int n,count;
	int sum = 0;
	
	printf("�����뵯������(1-10):");
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
		printf("ײ�ش���: %d",sum);
		delay(100000);
	}
	
	return 0;
}
