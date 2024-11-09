#include "ball.h"


//生成一个随机数，作为弹球的初始行位置 
int randX(void)
{
	int x;
	x = rand() % 22 + 1;
	return (x);
}

//生成一个随机数，作为弹球的初始列位置
int randY(void)
{
	int y;
	y = rand() % 87 + 1;
	return (y);
}

//生成0-359之间的一个随机数，作为弹球的运动方向
int randA(void)
{
	int a;
	a = rand() % 360;
	return (a);
}

//对num个弹球的数据进行初始化
void initBall(struct BALL ball[],int num)
{
	//初始化弹球“形状”列表 
	char c[Num]={'a','b','c','d','e','f','g','h','k','l'};
    int i, j;
    int used[10] = {0}; //用于记录每个字母的使用状况 
	
	for(i=0;i<num;i++)
	{
		int index;
        do
            index = rand() % 10;
        while (used[index]);
        used[index] = 1;
        
	    ball[i].body[0] = c[index];
        ball[i].body[1] = ball[i].body[0] - 32;
		ball[i].sel = rand()%2;
		ball[i].X = randX();
		ball[i].Y = randY();
		ball[i].wX = (int)(ball[i].X + 0.5);
		ball[i].wY = (int)(ball[i].Y + 0.5);
		ball[i].dX = cos((PI * randA()) / 180);
		ball[i].dY = sin((PI * randA()) / 180);
	}
}

//将图像数组初始化为背景图模式 
void initCharPicture(char cWin[24][90])
{
	int i,j;
	
    for (i = 1; i < 24; i++)
	{
        for (j = 1; j < 89; j++)
            cWin[i][j] = ' ';
    }
	
	for(i = 1;i < 24;i++)
	{
        cWin[i][0] = '*';
        cWin[i][88] = '*';
	}
	
	for(j = 0;j < 90;j++)
	{
        cWin[0][j] = '#';
        cWin[23][j] = '#';
	}
	
    cWin[23][89] = '\0';
		
	for(i=0;i < 24;i++)
		cWin[i][89] = '\n';
}

//根据num个弹球的最新位置，在屏幕上绘制出当前弹球位置的图像 
void redrawCWin(struct BALL ball[], int num)
{
	int i,j; 
	char cwin[24][90];
	initCharPicture(cwin);
	
	for(i = 0;i < num;i++)
		cwin[ball[i].wX][ball[i].wY] = ball[i].body[ball[i].sel];
		
	for(i = 0;i < 24;i++)
	{
		for(j = 0;j < 90;j++)
			printf("%c",cwin[i][j]);
	}
}

//计算弹球 k 和弹球 i 之间的距离
double distBetweenBalls(struct BALL ball[], int k, int i)
{
	double dist;
	dist=sqrt(pow(ball[k].X - ball[i].X,2) + pow(ball[k].Y - ball[i].Y,2));
	return (dist);
}

//将num个弹球根据各自的速度移动一次 
int moveBall(struct BALL ball[], int num)
{
	int i,j;
	int count = 0;
	for(i = 0;i < num;i++)
	{
		ball[i].X = ball[i].X + ball[i].dX;
		ball[i].Y = ball[i].Y + ball[i].dY;
	}
	
	for(i = 0;i < num;i++)
	{
		if(ball[i].X > 22)
		{
			putchar('\7');
			count++;
		}
		
		else if(ball[i].X > 22 || ball[i].X < 1)
		{
			ball[i].dX = -ball[i].dX;
			ball[i].X = ball[i].X + ball[i].dX;
		}
		
		else if(ball[i].Y > 88 || ball[i].Y < 1)
		{
			ball[i].dY = -ball[i].dY;
			ball[i].Y = ball[i].Y + ball[i].dY;
		}
		
		ball[i].wX = (int)(ball[i].X + 0.5);
		ball[i].wY = (int)(ball[i].Y+ 0.5);
		
		for(j = i + 1;j < num;j++)
		{
			double dist = distBetweenBalls(ball,i,j),b;
			if(dist<1)
			{
                ball[i].dX = -ball[i].dX;
                ball[i].dY = -ball[i].dY;
                ball[j].dX = -ball[j].dX;
                ball[j].dY = -ball[j].dY;
                ball[i].sel = (ball[i].sel + 1) % 2;
                ball[j].sel = (ball[j].sel + 1) % 2;
			}
		}	
		ball[i].wX = (int)( ball[i].X + 0.5);
		ball[i].wY = (int)( ball[i].Y+ 0.5);
		
	}
	return (count);
}

//延时一段时间 
void delay(int nMs)
{
	int i,j;
	for(i = 0;i < nMs;i++)
	{
		for(j = 0;j < 10000;j++)
			;
	}
}

//动画中隐藏光标 
void HideCursor() 
{
	CONSOLE_CURSOR_INFO cuesor_info={1,0};	//第二个值为0表示隐藏光标
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cuesor_info);
}
