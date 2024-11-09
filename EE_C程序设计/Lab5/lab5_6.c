#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(0));
    int score = 0;
    int i; 

    printf("这是一个20以内的加减法训练程序！\n");

    for(i = 1;i <= 10;i++)
	{
//		获得两个数和运算操作符 
        int num1 = rand() % 21;
        int num2;
        int operator = rand() % 2;
		 
        if (num1 == 20)
            num2 = rand() % (num1 + 1);
        else
            num2 = rand() % (21 - num1);
            

        printf("第%d题: ",i);

        if (operator == 0)
            printf("%d + %d = ", num1, num2);
        else
		{
            if (num1 < num2)
			{
                int temp = num1;
                num1 = num2;
                num2 = temp;
            }
            printf("%d - %d = ", num1, num2);
        }

        int answer,correctAnswer;
        scanf("%d", &answer);

        if(operator == 0)
            correctAnswer = num1 + num2;
        else
            correctAnswer = num1 - num2;

        if(answer == correctAnswer)
		{
            printf("回答正确！\n");
            score += 10;
        }
		else
            printf("回答错误！正确答案是 %d\n", correctAnswer);
    }

    printf("\n练习结束！你的总成绩是 %d 分\n", score);
    if (score == 100)
        printf("太棒了！你答对了所有题目，继续努力！\n");
    else
        printf("你可以做得更好，继续加油！\n");

    return 0;
}

