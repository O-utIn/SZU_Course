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
        printf("����ˣ������!\n\n");
        return 1;
    } 
	else
	{
        printf("�Բ���������Ŷ\n\n");
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
        printf("����ˣ������!\n\n");
        return 1;
    } 
	else
	{
        printf("�Բ���������Ŷ\n\n");
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
        printf("����ˣ������!\n\n");
        return 1;
    }
	else
	{
        printf("�Բ���������Ŷ\n\n");
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
    printf("\n%d �� %d = ", num1, num2);
    scanf("%d", &answer);
    if(answer == num1 / num2)
	{
        printf("����ˣ������!\n\n");
        return 1;
    }
	else
	{
        printf("�Բ���������Ŷ\n\n");
        return 0;
    }
}

int main()
{
    srand(time(0));

    printf("HELLO��СС����ң�������Ϸ���Ͽ�ʼ������\n");

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

        printf("�������Ϸ��?�����������밴 Y��y ��;���������Ϸ�밴���������");
        fflush(stdout);
        playAgain = getche();
        printf("\n");
    }
	while(playAgain == 'Y' || playAgain == 'y');

    printf("\n������Ϸ������:\n");
    printf("\t��������%d���ӷ��⣬���д��%d��\n", proNum[0], ansNum[0]);
    printf("\t��������%d�������⣬���д��%d��\n", proNum[1], ansNum[1]);
    printf("\t��������%d���˷��⣬���д��%d��\n", proNum[2], ansNum[2]);
    printf("\t��������%d�������⣬���д��%d��\n", proNum[3], ansNum[3]);

    return 0;
}
