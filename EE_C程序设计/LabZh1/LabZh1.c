#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main()
{
	int X,Y;
	char Location;
	printf("�����뽵��ص�ĵ������꣨X,Y����");
	scanf("%d%d",&X,&Y);
	fflush(stdin);
	
//	�ж���㲢�����ʾ�� 
	if(0 < X && X < 779 && 481 < Y && Y < 562) //����A�� 
	{
		printf("\n��������A����С�ĵ��ף�\n");
		Location = 'A';
	}
	else if(0 < X && X < 155 && 160 < Y && Y < 481) //����B�� 
	{
		printf("\n��������B����С�������壡\n");
		Location = 'B';
	}
	else if(0 < X && X < 779 && 0 < Y && Y < 160) //����C�� 
	{
		printf("\n��������C����С�ĵ��������\n");
		Location = 'C';		
	}
	else if(457 < X && X < 779 && 160 < Y && Y < 481) //����D�� 
	{
		printf("\n��������D�������Բ���������\n");
		Location = 'D';		
	}
	else if(155 < X && X < 457 && 160 < Y && Y < 481) //����O�� 
	{
		printf("\n��������O������ȥ������ʣ�\n");
		Location = 'O';
	}
	else if(X == 0 || X == 155 || X == 457 || X == 779 || Y == 0 || Y == 160 || Y == 481 || Y == 562) //��������߽������ 
	{
		printf("\n������������߽�����ϣ�\n");
		printf("\n����ʧ�ܣ�\n");	
		Location = 'F';
		exit(0);
	}
	else //������������
	{
		printf("\n���������������⣡\n");
		printf("\n����ʧ�ܣ�\n");	
		Location = 'F';
		exit(0);
	}
	
//	�ж�����ĵص㣬���ж�Ӧ�Ķ�����Ϸ 
	char button;
	srand(time(0));
	int W = 0 + rand() % 100; //����W�����ַ�ΧΪ0-99
	switch(Location)
	{
		case 'A': //����A��������Wa�����ַ�ΧΪ80-89
		{
			printf("\n����սʿ�����һ�����̣��������ж�����Ϸ\n"); 
			printf("����սʿ�����˶���W�������������¼����������:");
			button = getch();
			printf("%d\n",W); 
			int Wa= 80 + rand() % 10;
			printf("Ga����ͬʱҲ�����˶���Wa��������:%d\n",Wa); 
			if(W < Wa)
				printf("\n���ױ�ը������ʧ�ܣ�\n");
			else
				printf("\n�ɹ�����������O����������ʳɹ���\n");
			break;
		}
		case 'B': //����B��������Wb�����ַ�ΧΪ45-55
		{
			printf("\n����սʿ�����һ�����̣��������ж�����Ϸ\n"); 
			printf("����սʿ�����˶���W�������������¼����������:");
			button = getch();
			printf("%d\n",W); 
			int Wb = 45 + rand() % 11;
			printf("Gb����ͬʱҲ�����˶���Wa��������:%d\n",Wb); 
			if(W < Wb)
				printf("\n�������壬����ʧ�ܣ�\n");
			else
				printf("\n�ɹ�����������O����������ʳɹ���\n");
			break;
		}
		case 'C': //����C��������Wc�����ַ�ΧΪ60-69
		{
			printf("\n����սʿ�����һ�����̣��������ж�����Ϸ\n"); 
			printf("����սʿ�����˶���W�������������¼����������:");
			button = getch();
			printf("%d\n",W); 
			int Wc = 60 + rand() % 10;
			printf("Gc����ͬʱҲ�����˶���Wa��������:%d\n",Wc); 
			if(W < Wc)
				printf("\n�⵽����������ʧ�ܣ�\n");
			else
				printf("\n�ɹ�����������O����������ʳɹ���\n");
			break;
		}
		case 'D': //����D��������Wd�����ַ�ΧΪ10-19
		{
			printf("\n����սʿ�����һ�����̣��������ж�����Ϸ\n"); 
			printf("����սʿ�����˶���W�������������¼����������:");
			button = getch();
			printf("%d\n",W); 
			int Wd = 10 + rand() % 10;
			printf("Gd����ͬʱҲ�����˶���Wa��������:%d\n",Wd); 
			if(W < Wd)
				printf("\n������ʧ������ʧ�ܣ�\n");
			else
				printf("\n�ɹ�����������O����������ʳɹ���\n");
			break;
		default:
			printf("\n������ɣ��ȳ����ʣ�\n");
			break; 
		}
	}
	
	return 0;
}


