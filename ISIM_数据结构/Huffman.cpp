#include <bits/stdc++.h>
using namespace std;

typedef struct HuffmanNode
{
	char data;
	int weight;
	int lchild,rchild,parent;
}HTNode,*HuffmanTree;

void select(HuffmanTree t,int i,int& s1,int& s2)
{
	int j;
	//定位第一个根节点下标起点
	for(j = 0;j < i;j++)
		if(t[j].parent == -1)
			break;
	//求s1
    for(s1 = j,j++;j < i;j++)
		if(t[j].parent == -1 && t[j].weight < t[s1].weight)
			s1 = j;

	//定位第二个根节点下标起点 
	for(j = 0;j < i;j++)
		if(t[j].parent == -1 && j != s1)//避开s1
			break;
    //求s2
	for(s2 = j,j++;j < i;j++)
		if(t[j].parent == -1 && j != s1 && t[j].weight < t[s2].weight)
			s2 = j;
}

void CreateHT(HuffmanTree& HT,int n)
{
    //输入数据及权重
	char* input_char = new char[n];
	int*  input_weight = new int[n];
	cout << "please input char:" << endl;
	for(int i = 0;i < n;i++)
		cin >> input_char[i];
	cout << "please input weight:" << endl;
	for(int i = 0;i < n;i++)
		cin >> input_weight[i];

	int m = 2 * n - 1;
	HT = new HTNode[m];

    //初始化哈夫曼树
	for(int i = 0;i < n;i++)
    {
		HT[i].data = input_char[i];
		HT[i].weight = input_weight[i];
		HT[i].parent = -1;
		HT[i].lchild = -1;
		HT[i].rchild = -1;
	}
	for(int i = 0;i < m;i++)
		HT[i].parent = -1;
	
    //创建哈夫曼树
	int s1,s2;
	for(int i = n;i < m;i++)
    {
		select(HT,i,s1,s2);
		HT[s1].parent = HT[s2].parent = i;
		HT[i].lchild = s1;
		HT[i].rchild = s2;
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}
	
	delete input_char;
	delete input_weight;
}

void Code(HuffmanTree HT,int n,int i,char* code)
{
	char *temp = new char[n];
	
    //从后往前回溯对字符进行编码
	temp[n-1] = '\0';
	int start = n - 1;

	int parent = HT[i].parent;
	int p = i;//初始从当前点出发
	while(parent != -1)
    {
		if(HT[parent].lchild == p)
			temp[--start] = '0';
		else
			temp[--start] = '1';
		p = parent;
		parent = HT[parent].parent;//从父亲上嗍
	}
	strcpy_s(code,strlen(&temp[start])+1,&(temp[start]));
	
	delete temp;
}

int main(int argc, char **argv)
{
	int n;
	HuffmanTree HT;
	cout << "please input the number of char:";
	cin >> n;

	CreateHT(HT,n);
	//显示数组
	for(int t = 0;t < 2 * n - 1;t++)
		cout << HT[t].weight << " " << HT[t].parent << " " << HT[t].lchild << " " << HT[t].rchild << endl;

	//求第i个字符的编码
	char* code = new char[n];

	for(int i = 0;i < n;i++)
	{
		Code(HT,n,i,code);
		cout << HT[i].data << "->" << code << endl;
	}

	delete code;
	delete HT;

	return 0;
}

