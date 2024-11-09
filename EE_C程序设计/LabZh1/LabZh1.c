#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main()
{
	int X,Y;
	char Location;
	printf("请输入降落地点的地理坐标（X,Y）：");
	scanf("%d%d",&X,&Y);
	fflush(stdin);
	
//	判断落点并输出提示语 
	if(0 < X && X < 779 && 481 < Y && Y < 562) //落在A区 
	{
		printf("\n你现在在A区，小心地雷！\n");
		Location = 'A';
	}
	else if(0 < X && X < 155 && 160 < Y && Y < 481) //落在B区 
	{
		printf("\n你现在在B区，小心有陷阱！\n");
		Location = 'B';
	}
	else if(0 < X && X < 779 && 0 < Y && Y < 160) //落在C区 
	{
		printf("\n你现在在C区，小心敌人埋伏！\n");
		Location = 'C';		
	}
	else if(457 < X && X < 779 && 160 < Y && Y < 481) //落在D区 
	{
		printf("\n你现在在D区，可以补充武器！\n");
		Location = 'D';		
	}
	else if(155 < X && X < 457 && 160 < Y && Y < 481) //落在O区 
	{
		printf("\n你现在在O区，快去解救人质！\n");
		Location = 'O';
	}
	else if(X == 0 || X == 155 || X == 457 || X == 779 || Y == 0 || Y == 160 || Y == 481 || Y == 562) //落在区域边界电网上 
	{
		printf("\n你落在了区域边界电网上！\n");
		printf("\n任务失败！\n");	
		Location = 'F';
		exit(0);
	}
	else //落在任务区外
	{
		printf("\n你落在了任务区外！\n");
		printf("\n任务失败！\n");	
		Location = 'F';
		exit(0);
	}
	
//	判断落入的地点，进行对应的赌盘游戏 
	char button;
	srand(time(0));
	int W = 0 + rand() % 100; //赌盘W的数字范围为0-99
	switch(Location)
	{
		case 'A': //落在A区，赌盘Wa的数字范围为80-89
		{
			printf("\n超级战士获得了一个赌盘，即将进行赌盘游戏\n"); 
			printf("超级战士按下了赌盘W的启动键（按下键盘任意键）:");
			button = getch();
			printf("%d\n",W); 
			int Wa= 80 + rand() % 10;
			printf("Ga门卫同时也按下了赌盘Wa的启动键:%d\n",Wa); 
			if(W < Wa)
				printf("\n地雷爆炸，任务失败！\n");
			else
				printf("\n成功进入人质区O区，解救人质成功！\n");
			break;
		}
		case 'B': //落在B区，赌盘Wb的数字范围为45-55
		{
			printf("\n超级战士获得了一个赌盘，即将进行赌盘游戏\n"); 
			printf("超级战士按下了赌盘W的启动键（按下键盘任意键）:");
			button = getch();
			printf("%d\n",W); 
			int Wb = 45 + rand() % 11;
			printf("Gb门卫同时也按下了赌盘Wa的启动键:%d\n",Wb); 
			if(W < Wb)
				printf("\n掉入陷阱，任务失败！\n");
			else
				printf("\n成功进入人质区O区，解救人质成功！\n");
			break;
		}
		case 'C': //落在C区，赌盘Wc的数字范围为60-69
		{
			printf("\n超级战士获得了一个赌盘，即将进行赌盘游戏\n"); 
			printf("超级战士按下了赌盘W的启动键（按下键盘任意键）:");
			button = getch();
			printf("%d\n",W); 
			int Wc = 60 + rand() % 10;
			printf("Gc门卫同时也按下了赌盘Wa的启动键:%d\n",Wc); 
			if(W < Wc)
				printf("\n遭到伏击，任务失败！\n");
			else
				printf("\n成功进入人质区O区，解救人质成功！\n");
			break;
		}
		case 'D': //落在D区，赌盘Wd的数字范围为10-19
		{
			printf("\n超级战士获得了一个赌盘，即将进行赌盘游戏\n"); 
			printf("超级战士按下了赌盘W的启动键（按下键盘任意键）:");
			button = getch();
			printf("%d\n",W); 
			int Wd = 10 + rand() % 10;
			printf("Gd门卫同时也按下了赌盘Wa的启动键:%d\n",Wd); 
			if(W < Wd)
				printf("\n武器丢失，任务失败！\n");
			else
				printf("\n成功进入人质区O区，解救人质成功！\n");
			break;
		default:
			printf("\n任务完成，救出人质！\n");
			break; 
		}
	}
	
	return 0;
}


