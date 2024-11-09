#include <stdio.h>

#define Num_Of_Judges 5  //设置评委数量 


//计算最终得分函数 
void SCORE(int Num_Of_Players,int Score[Num_Of_Players][Num_Of_Judges],float score[])
{
	int i = 0;
	int j = 0;
	int k = 0;
	
	//使用冒泡排序给评委打的分进行排序 
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
		//去掉最高分和最低分 
		for (j = 1; j < Num_Of_Judges - 1; j++)
		{
			score[i] += Score[i][j];
		}
		//计算平均分 
		score[i] = 1.0 * score[i] / (Num_Of_Judges - 2);
    }
    printf("\n");
}


//使用打擂台算法对选手排名排序函数 
void Rank(float* score, int* rank,int Num_Of_Players)
{
	int i = 0;
	int j = 0;
	int No = 1;//名次
	int num = 0;//记已排名人数
	//从前往后依次排名
	for (i = 0; i < Num_Of_Players; i++)
	{
		float max = 0;
		//找到最大得分
		for (j = 0; j < Num_Of_Players; j++)
		{
			if (score[j] > max && rank[j] == 0)
				max = score[j];
		}
		//开始排名
		for (j = 0; j < Num_Of_Players; j++)
		{
			if (score[j] == max)
			{
				rank[j] = No;//找第一名、第二名······ 
				num++;
			}
		}
		if (num < Num_Of_Players)
			No++;	
	}
}


int main()
{
	//获取选手数量 
	printf("\t本程序为选美大奖赛半决赛打分系统\n\n");
	int Num_Of_Players;

	printf("请输入选手数量(4-10)：");
	scanf("%d",&Num_Of_Players);
	if(Num_Of_Players < 4)
	{
		Num_Of_Players = 4;
		printf("检测到输入的选手数量为非法输入，已修正为最小值“4”");
	}
	else if(Num_Of_Players > 10)
	{
		Num_Of_Players = 10;
		printf("检测到输入的选手数量为非法输入，已修正为最大值“10”");
	}
	printf("\n选手编号为:");


	int i,j;
	for(i = 0;i < Num_Of_Players;i++)
	{
		printf("%d ",i + 1);
	}
	
	int Score[Num_Of_Players][Num_Of_Judges];
	float score[15] = { 0 };
    int rank[15] = { 0 };
    
    printf("\n\n请 %d 位评委对以下 %d 位选手评分（满分为10分）:\n",Num_Of_Judges,Num_Of_Players);
    printf("【每位评委之间的打分以空格分隔，回车确认对当前选手的打分】\n\n"); 


	
	//输入评分 
    for (i = 0; i < Num_Of_Players; i++)
	{
		printf("%2d 号选手：",i + 1);
		for (j = 0; j < Num_Of_Judges; j++)
		{
			scanf("%d", &Score[i][j]);
		}
    }
    
    //调用计算和排序函数 
    SCORE(Num_Of_Players,Score,score);
	Rank(score,rank,Num_Of_Players);
	
	//打印结果 
    printf("经过评委打分后(去掉一个最高分，去掉一个最低分，取平均分，保留一位小数)\n") ; 
	printf("下面是本次比赛的结果：\n\n");
	
    printf("PlayerNo Score Rank\n");
    for (i = 0; i < Num_Of_Players; i++)
	{
    	printf("%4d\t  %3.1f    %d\n", i + 1, score[i], rank[i]);
    }

	return 0;
}
