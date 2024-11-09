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
	char body[2];	//两个不同的字符，分别代表两个不同颜色的球
	int sel;		//当前球的颜色。0表示第一种颜色，1表示第二种颜色
	int wX;	//在二维数组中，球在x方向的实际显示位置（整数） 
	int wY;	//在二维数组中，球在y方向的实际显示位置（整数） 
	double X;	//球在x方向的精确位置（实数）
	double Y;	//球在y方向的精确位置（实数） 
	double dX;	//球在x方向的速度（实数） 
	double dY;	//球在y方向的速度（实数）
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
