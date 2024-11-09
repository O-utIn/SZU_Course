#ifndef BALL_H
#define BALL_H


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <Windows.h>


#define Num 10
#define PI 3.1415926


struct BALL
{
	char body[2];	//������ͬ���ַ����ֱ����������ͬ��ɫ����
	int sel;		//��ǰ�����ɫ��0��ʾ��һ����ɫ��1��ʾ�ڶ�����ɫ
	int wX;	//�ڶ�ά�����У�����x�����ʵ����ʾλ�ã������� 
	int wY;	//�ڶ�ά�����У�����y�����ʵ����ʾλ�ã������� 
	double X;	//����x����ľ�ȷλ�ã�ʵ����
	double Y;	//����y����ľ�ȷλ�ã�ʵ���� 
	double dX;	//����x������ٶȣ�ʵ���� 
	double dY;	//����y������ٶȣ�ʵ����
};


void initBall(struct BALL ball[],int num);
int moveBall(struct BALL ball[], int num);
void redrawCWin(struct BALL ball[], int num); 
void initCharPicture(char cWin[24][90]);
void delay(int nMs);
int randX(void);
int randY(void);
int randA(void);
double distBetweenBalls(struct BALL ball[], int k, int i);
void HideCursor();

#endif
