#include <bits/stdc++.h>
using namespace std;

const int MAX = 100;
enum GraphType{DG,UG,DN,UN}; //图的类型定义:有向图，无向图，有向网，无向网
typedef char VertexType;
bool visited[MAX];

typedef struct
{
    VertexType vexs[MAX]; //顶点表
    int arcs[MAX][MAX]; //邻接矩阵
    int vexnum,arcnum; //顶点数和边数
    GraphType kind; //图的类型
}MGraph;

void CreateMatrix(MGraph& G)
{
    G.vexnum = 7; //7个顶点
    G.arcnum = 12; //12条边
    char temp[] = {'A','B','C','D','E','F','G'};

    for(int i = 0;i < G.vexnum;i++)
        G.vexs[i] = temp[i];

    //初始化邻接矩阵
    for(int i = 0;i < G.vexnum;i++)
        for(int j = 0;j < G.vexnum;j++)
            G. arcs[i][j] = 0;

    G.arcs[0][1] = G.arcs[1][0] = 1;
    G.arcs[0][5] = G.arcs[5][0] = 1;
    G.arcs[0][6] = G.arcs[6][0] = 1;
    G.arcs[1][2] = G.arcs[2][1] = 1;
    G.arcs[1][5] = G.arcs[5][1] = 1;
    G.arcs[2][5] = G.arcs[5][2] = 1;
    G.arcs[2][4] = G.arcs[4][2] = 1;
    G.arcs[2][3] = G.arcs[3][2] = 1;
    G.arcs[3][4] = G.arcs[4][3] = 1;
    G.arcs[4][6] = G.arcs[6][4] = 1;
    G.arcs[4][5] = G.arcs[5][4] = 1;
    G.arcs[6][5] = G.arcs[5][6] = 1;

    G.kind=UG;
}

void DFS(MGraph G,int v)
{
    cout << G.vexs[v];
    visited[v] = true;
    for(int i = 0;i<G.vexnum;i++)
    {
        if(G.arcs[v][i] != 0 && !visited[i])
            DFS(G,i);
    }
}

void DFS_Graph(MGraph G)
{
    cout << "Output is:";
    for(int i = 0;i < G.vexnum;i++)
        visited[i] = false;
    
    for(int i= 0;i < G.vexnum;i++)
    {
        if(!visited[i])
            DFS(G, i);
    }
}

int main(int argc, char **argv)
{
    MGraph G;
    CreateMatrix(G);
    DFS_Graph(G);
}