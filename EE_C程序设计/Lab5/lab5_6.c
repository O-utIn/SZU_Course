#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(0));
    int score = 0;
    int i; 

    printf("����һ��20���ڵļӼ���ѵ������\n");

    for(i = 1;i <= 10;i++)
	{
//		�������������������� 
        int num1 = rand() % 21;
        int num2;
        int operator = rand() % 2;
		 
        if (num1 == 20)
            num2 = rand() % (num1 + 1);
        else
            num2 = rand() % (21 - num1);
            

        printf("��%d��: ",i);

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
            printf("�ش���ȷ��\n");
            score += 10;
        }
		else
            printf("�ش������ȷ���� %d\n", correctAnswer);
    }

    printf("\n��ϰ����������ܳɼ��� %d ��\n", score);
    if (score == 100)
        printf("̫���ˣ�������������Ŀ������Ŭ����\n");
    else
        printf("��������ø��ã��������ͣ�\n");

    return 0;
}

