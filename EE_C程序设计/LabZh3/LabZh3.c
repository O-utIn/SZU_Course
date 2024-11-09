#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int randN1N2(int rN1, int rN2)
{
    return rand() % (rN2 - rN1 + 1) + rN1;
}

char randOper4()
{
    char operators[] = {'+', '-', '*', '/'};
    return operators[rand() % 4];
}

int randLmt100()
{
    return randN1N2(0, 99);
}

int addLmt100(int num1, int num2)
{
    int answer;
    printf("\n%d + %d = ", num1, num2);
    scanf("%d", &answer);
    if(answer == num1 + num2)
	{
        printf("答对了，你真棒!\n\n");
        return 1;
    } 
	else
	{
        printf("对不起，你答错了哦\n\n");
        return 0;
    }
}

int subLmt100(int num1, int num2)
{
    int answer;
    int numM1 = num1 > num2 ? num1 : num2;
    int numM2 = num1 > num2 ? num2 : num1;
    printf("\n%d - %d = ", numM1, numM2);
    scanf("%d", &answer);
    if(answer == numM1 - numM2)
	{
        printf("答对了，你真棒!\n\n");
        return 1;
    } 
	else
	{
        printf("对不起，你答错了哦\n\n");
        return 0;
    }
}

int mulLmt100(int num1, int num2)
{
    int answer;
    printf("\n%d * %d = ", num1, num2);
    scanf("%d", &answer);
    if(answer == num1 * num2)
	{
        printf("答对了，你真棒!\n\n");
        return 1;
    }
	else
	{
        printf("对不起，你答错了哦\n\n");
        return 0;
    }
}

int divLmt100()
{
    int num1, num2;
    do
	{
        num1 = randLmt100();
        num2 = randLmt100();
    }
	while(num2 == 0 || num1 % num2 != 0);

    int answer;
    printf("\n%d ÷ %d = ", num1, num2);
    scanf("%d", &answer);
    if(answer == num1 / num2)
	{
        printf("答对了，你真棒!\n\n");
        return 1;
    }
	else
	{
        printf("对不起，你答错了哦\n\n");
        return 0;
    }
}

int main()
{
    srand(time(0));

    printf("HELLO，小小口算家，口算游戏马上开始。。。\n");

    int proNum[4] = {0, 0, 0, 0};
    int ansNum[4] = {0, 0, 0, 0};

    char playAgain;
    do
	{
        char operation = randOper4();
        int num1 = randLmt100();
        int num2 = randLmt100();

        switch (operation)
		{
            case '+':
                proNum[0]++;
                ansNum[0] += addLmt100(num1, num2);
                break;
            case '-':
                proNum[1]++;
                ansNum[1] += subLmt100(num1, num2);
                break;
            case '*':
                proNum[2]++;
                ansNum[2] += mulLmt100(num1, num2);
                break;
            case '/':
                proNum[3]++;
                ansNum[3] += divLmt100();
                break;
        }

        printf("想继续游戏吗?如果想继续，请按 Y或y 键;若想结束游戏请按其他任意键");
        fflush(stdout);
        playAgain = getche();
        printf("\n");
    }
	while(playAgain == 'Y' || playAgain == 'y');

    printf("\n本次游戏完成情况:\n");
    printf("\t共计算了%d道加法题，其中答对%d道\n", proNum[0], ansNum[0]);
    printf("\t共计算了%d道减法题，其中答对%d道\n", proNum[1], ansNum[1]);
    printf("\t共计算了%d道乘法题，其中答对%d道\n", proNum[2], ansNum[2]);
    printf("\t共计算了%d道除法题，其中答对%d道\n", proNum[3], ansNum[3]);

    return 0;
}
