#include <bits/stdc++.h>
using namespace std;

#define MAXV 50 //最大顶点数
typedef struct
{
	int no;//顶点编号
	//InfoType info;//顶点的其他信息
}VertexType;//顶点类型

typedef struct
{
	int edges[MAXV][MAXV];//邻接矩阵的边数组
	int n, e;//顶点数，边数
	VertexType vexs[MAXV];//存放顶点信息
}MGraph;//图邻接矩阵类型
#define INF 32767//表示无穷大



void Prim(MGraph g, int v)//普利姆算法（参数：邻接矩阵，起点（即第一个生成的点，可随便取））
{
	int lowcost[MAXV], closest[MAXV], i, min, j, k;
 
	/***初始化lowcost数组，closest数组(即从起点开始设置lowcost数组，closest数组相应的值，以便后续生成使用)***/
	for (i = 0; i < g.n; i++)//赋初值，即将closest数组都赋为第一个节点v，lowcost数组赋为第一个节点v到各节点的权重
	{
		closest[i] = v;
		lowcost[i] = g.edges[v][i];//g.edges[v][i]的值指的是节点v到i节点的权重
	}
 
	/**********************************开始生成其他的节点*********************************/
	for (i = 1; i < g.n; i++)//接下来找剩下的n-1个节点（g.n是图的节点个数）
	{
 
		/*****找到一个节点，该节点到已选节点中的某一个节点的权值是当前最小的*****/
		min = INF;//INF表示正无穷（每查找一个节点，min都会重新更新为INF，以便获取当前最小权重的节点）
		for (j = 0; j < g.n; j++)//遍历所有节点
		{
			if (lowcost[j] != 0 && lowcost[j] < min)//若该节点还未被选且权值小于之前遍历所得到的最小值
			{
				min = lowcost[j];//更新min的值
				k = j;//记录当前最小权重的节点的编号
			}
		}
 
		/****************输出被连接节点与连接节点，以及它们的权值***************/
		printf("边(%d,%d)权为:%d\n", closest[k], k, min);
 
		/***********更新lowcost数组，closest数组，以便生成下一个节点************/
		lowcost[k] = 0;//表明k节点已被选了(作标记)
		//选中一个节点完成连接之后，作数组相应的调整
		for (j = 0; j < g.n; j++)//遍历所有节点
		{
			/* if语句条件的说明：
			 * （1）g.edges[k][j] != 0是指k！=j，即跳过自身的节点
			 * （2）g.edges[k][j]是指刚被选的节点k到节点j的权重，lowcost[j]是指之前遍历的所有节点与j节点的最小权重。若g.edges[k][j] < lowcost[j],则说明当前刚被选的节点k与节点j之间存在更小的权重，则需要更新
			 * （3）有人会问：为什么只跳过掉自身的节点（即k==j），而不跳过所有的已选节点？当然我们可以在if语句条件中增加跳过所有的已选节点的条件（即lowcost[j] == 0），而在本程序中我们只跳过了自身的节点？（注意：我们假设图中的边的权值大于0）但其实不是，g.edges[k][j] < lowcost[j]条件已包含跳过所有的已选节点，原因是在邻接矩阵中权值为0是最小的，即g.edges[k][j]>=0，而已选节点满足lowcost[j] == 0，则已选节点j是不满足g.edges[k][j] < lowcost[j]，则会被跳过
			 */
			if (g.edges[k][j] != 0 && g.edges[k][j] < lowcost[j])
			{
				//更新lowcost数组，closest数组
				lowcost[j] = g.edges[k][j];//更新权重，使其当前最小
				closest[j] = k;//进入到该if语句里，说明刚选的节点k与当前节点j有更小的权重，则closest[j]的被连接节点需作修改为k
			}
		}
	}
}

void CreateMatrix(MGraph& G) {
    G.n = 7; //7个顶点
    G.e = 12; //12条边
    char temp[] = {'A','B','C','D','E','F','G'};

    for (int i = 0; i < G.n; i++) {
        G.vexs[i] = temp[i];
    }

    //初始化邻接矩阵
    for (int i = 0; i < G.n; i++) {
        for (int j = 0; j < G.n; j++) {
            G.edges[i][j] = 0;
        }
    }

    G.edges[0][1] = G.edges[1][0] = 1;
    G.edges[0][5] = G.edges[5][0] = 1;
    G.edges[0][6] = G.edges[6][0] = 1;
    G.edges[1][2] = G.edges[2][1] = 1;
    G.edges[1][5] = G.edges[5][1] = 1;
    G.edges[2][5] = G.edges[5][2] = 1;
    G.edges[2][4] = G.edges[4][2] = 1;
    G.edges[2][3] = G.edges[3][2] = 1;
    G.edges[3][4] = G.edges[4][3] = 1;
    G.edges[4][6] = G.edges[6][4] = 1;
    G.edges[4][5] = G.edges[5][4] = 1;
    G.edges[6][5] = G.edges[5][6] = 1;

    G.kind = UG;
}

int main(int argc, char **argv)
{
    MGraph g;
    CreateMatrix(g);
    Prim(g, 0);

    return 0;
}