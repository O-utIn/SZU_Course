#include "ball.h"


//����һ�����������Ϊ����ĳ�ʼ��λ�� 
int randX(void)
{
	int x;
	x = rand() % 22 + 1;
	return (x);
}

//����һ�����������Ϊ����ĳ�ʼ��λ��
int randY(void)
{
	int y;
	y = rand() % 87 + 1;
	return (y);
}

//����0-359֮���һ�����������Ϊ������˶�����
int randA(void)
{
	int a;
	a = rand() % 360;
	return (a);
}

//��num����������ݽ��г�ʼ��
void initBall(struct BALL ball[],int num)
{
	//��ʼ��������״���б� 
	char c[Num]={'a','b','c','d','e','f','g','h','k','l'};
    int i, j;
    int used[10] = {0}; //���ڼ�¼ÿ����ĸ��ʹ��״�� 
	
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

//��ͼ�������ʼ��Ϊ����ͼģʽ 
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

//����num�����������λ�ã�����Ļ�ϻ��Ƴ���ǰ����λ�õ�ͼ�� 
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

//���㵯�� k �͵��� i ֮��ľ���
double distBetweenBalls(struct BALL ball[], int k, int i)
{
	double dist;
	dist=sqrt(pow(ball[k].X - ball[i].X,2) + pow(ball[k].Y - ball[i].Y,2));
	return (dist);
}

//��num��������ݸ��Ե��ٶ��ƶ�һ�� 
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

//��ʱһ��ʱ�� 
void delay(int nMs)
{
	int i,j;
	for(i = 0;i < nMs;i++)
	{
		for(j = 0;j < 10000;j++)
			;
	}
}

//���������ع�� 
void HideCursor() 
{
	CONSOLE_CURSOR_INFO cuesor_info={1,0};	//�ڶ���ֵΪ0��ʾ���ع��
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cuesor_info);
}
