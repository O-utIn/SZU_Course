#include <iostream>
using namespace std;

const int MAXSIZE = 100;
typedef int ElemType;
typedef struct
{
	ElemType data[MAXSIZE];
	int length;
}SqeList;

void InitSL(SqeList&); //初始化顺序表
void CreateSL(SqeList&,int); //生成顺序表
void ShowSL(SqeList); //遍历输出顺序表
int LengthSL(SqeList); //查询顺序表长度
int FindSL(SqeList,ElemType); //按值查找元素
void InsertSL(SqeList&,int,ElemType); //插入元素
void DeleteSL(SqeList&,int); //删除元素

int main(int argc, char **argv)
{
	SqeList SL; 
	InitSL(SL);

	int len = 0; 
	cout << "请输入顺序表长度:";
	cin >> len;
	CreateSL(SL,len);

	ShowSL(SL); 

	cout << "顺序表的长度为:" << LengthSL(SL) << endl; 

	ElemType Finde; 
	cout << "请输入需要查询的元素ֵ:";
	cin >> Finde;
	cout <<  "该元素是顺序表中第" << FindSL(SL,Finde) << "个元素(返回0则说明顺序表中不存在该元素)" << endl;

	int index;
	ElemType Inserte;
	cout << "请输入将在第几个位置插入新元素:";
	cin >> index;
	cout << "请输入需要插入的元素值:";
	cin >> Inserte;
	InsertSL(SL,index,Inserte);

	int DeleteIndex;
	cout << "请输入需要删除第几个元素:";
	cin >> DeleteIndex;
	DeleteSL(SL,DeleteIndex);

	return 0;
}

//初始化顺序表
void InitSL(SqeList& SL)
{
	SL.length = 0;
}

//创建顺序表
void CreateSL(SqeList& SL,int len)
{
	if(len > MAXSIZE)
		cout << "顺序表已满" << endl;
	SL.length = len;
	for(int i = 0;i < len;i++)
	{
		cout << "请输入索引为" << i << "的元素（该元素是第" << i+1 << "个元素）:";
		cin >> SL.data[i];
	}
}

//输出顺序表
void ShowSL(SqeList SL)
{
	cout << "顺序表为:";
	for(int i = 0;i< SL.length;i++)
		cout << SL.data[i] << " ";
	cout << endl;
}

//查询顺序表长度
int LengthSL(SqeList SL)
{
	return SL.length;
}

//查找元素
int FindSL(SqeList SL,ElemType e)
{
	if(SL.length == 0)
	{
		cout << "顺序表已空，操作失败";
		return -1;
	}
	else
	{
		for(int i = 0;i< SL.length;i++)
		{
			if(SL.data[i] == e)
				return i+1;
		}
		return 0;
	}
}

//插入元素
void InsertSL(SqeList& SL,int index,ElemType e)
{
	if(SL.length + 1 > MAXSIZE)
		cout << "顺序表已满，操作失败" << endl;
	else
	{
		SL.length += 1;
		for(int i = SL.length - 1;i >= index;i--)
		{
			SL.data[i] = SL.data[i - 1];
		}
		SL.data[index - 1] = e;
	}
	ShowSL(SL);
}

//删除元素
void DeleteSL(SqeList& SL,int index)
{
	if(SL.length == 0)
		cout << "顺序表已空，操作失败" << endl;
	else
	{
		for(int i = index;i < SL.length;i++)
		{
			SL.data[i - 1] = SL.data[i];
		}
		SL.length--;
		ShowSL(SL);
	}
}
