#include <stdio.h>

#define Num_Of_Judges 5  //������ί���� 


//�������յ÷ֺ��� 
void SCORE(int Num_Of_Players,int Score[Num_Of_Players][Num_Of_Judges],float score[])
{
	int i = 0;
	int j = 0;
	int k = 0;
	
	//ʹ��ð���������ί��ķֽ������� 
	for(i = 0; i < Num_Of_Players; i++)
	{
		for(j = 0; j < Num_Of_Judges - 1; j++)
		{
			for(k = 0; k < Num_Of_Judges - 1 - j; k++)
			{
				if(Score[i][k] > Score[i][k + 1])
				{
					int temp = Score[i][k];
					Score[i][k] = Score[i][k + 1];
					Score[i][k + 1] = temp;
				}
			}
		}
	}
	
	for (i = 0; i < Num_Of_Players; i++)
	{
		//ȥ����߷ֺ���ͷ� 
		for (j = 1; j < Num_Of_Judges - 1; j++)
		{
			score[i] += Score[i][j];
		}
		//����ƽ���� 
		score[i] = 1.0 * score[i] / (Num_Of_Judges - 2);
    }
    printf("\n");
}


//ʹ�ô���̨�㷨��ѡ������������ 
void Rank(float* score, int* rank,int Num_Of_Players)
{
	int i = 0;
	int j = 0;
	int No = 1;//����
	int num = 0;//������������
	//��ǰ������������
	for (i = 0; i < Num_Of_Players; i++)
	{
		float max = 0;
		//�ҵ����÷�
		for (j = 0; j < Num_Of_Players; j++)
		{
			if (score[j] > max && rank[j] == 0)
				max = score[j];
		}
		//��ʼ����
		for (j = 0; j < Num_Of_Players; j++)
		{
			if (score[j] == max)
			{
				rank[j] = No;//�ҵ�һ�����ڶ��������������� 
				num++;
			}
		}
		if (num < Num_Of_Players)
			No++;	
	}
}


int main()
{
	//��ȡѡ������ 
	printf("\t������Ϊѡ��������������ϵͳ\n\n");
	int Num_Of_Players;

	printf("������ѡ������(4-10)��");
	scanf("%d",&Num_Of_Players);
	if(Num_Of_Players < 4)
	{
		Num_Of_Players = 4;
		printf("��⵽�����ѡ������Ϊ�Ƿ����룬������Ϊ��Сֵ��4��");
	}
	else if(Num_Of_Players > 10)
	{
		Num_Of_Players = 10;
		printf("��⵽�����ѡ������Ϊ�Ƿ����룬������Ϊ���ֵ��10��");
	}
	printf("\nѡ�ֱ��Ϊ:");


	int i,j;
	for(i = 0;i < Num_Of_Players;i++)
	{
		printf("%d ",i + 1);
	}
	
	int Score[Num_Of_Players][Num_Of_Judges];
	float score[15] = { 0 };
    int rank[15] = { 0 };
    
    printf("\n\n�� %d λ��ί������ %d λѡ�����֣�����Ϊ10�֣�:\n",Num_Of_Judges,Num_Of_Players);
    printf("��ÿλ��ί֮��Ĵ���Կո�ָ����س�ȷ�϶Ե�ǰѡ�ֵĴ�֡�\n\n"); 


	
	//�������� 
    for (i = 0; i < Num_Of_Players; i++)
	{
		printf("%2d ��ѡ�֣�",i + 1);
		for (j = 0; j < Num_Of_Judges; j++)
		{
			scanf("%d", &Score[i][j]);
		}
    }
    
    //���ü���������� 
    SCORE(Num_Of_Players,Score,score);
	Rank(score,rank,Num_Of_Players);
	
	//��ӡ��� 
    printf("������ί��ֺ�(ȥ��һ����߷֣�ȥ��һ����ͷ֣�ȡƽ���֣�����һλС��)\n") ; 
	printf("�����Ǳ��α����Ľ����\n\n");
	
    printf("PlayerNo Score Rank\n");
    for (i = 0; i < Num_Of_Players; i++)
	{
    	printf("%4d\t  %3.1f    %d\n", i + 1, score[i], rank[i]);
    }

	return 0;
}
