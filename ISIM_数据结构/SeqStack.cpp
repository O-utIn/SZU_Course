#include <iostream>
using namespace std;
const int MAXSIZE=100;

typedef int ElemType;
typedef struct
{
	ElemType data[MAXSIZE];
	int top;
}SeqStack;

void InitSeqStack(SeqStack&); //初始化顺序栈
void PushSeqStack(SeqStack&,ElemType); //入栈
ElemType PopSeqStack(SeqStack&); //出栈
ElemType TopElemSeqStack(SeqStack&); //取栈顶元素
bool EmptySeqStack(SeqStack&); //判断满栈
bool NoneSeqStack(SeqStack&); //判断空栈

int main(int argc, char **argv)
{
	SeqStack S;
	InitSeqStack(S);

	PushSeqStack(S,9);
	PushSeqStack(S,16);
	PushSeqStack(S,27);
	cout << "正在对栈顶元素" << PopSeqStack(S) << "进行出栈操作" << endl;
	cout << "正在对栈顶元素" << PopSeqStack(S) << "进行出栈操作" << endl;

	cout << "栈顶元素为" << TopElemSeqStack(S) << endl;

	cout << "栈是否已满(返回1则已满，返回0未满):" << EmptySeqStack(S) << endl;
	cout << "栈是否为空(返回1则已空，返回0未空):" << NoneSeqStack(S) << endl;

	return 0;
}

void InitSeqStack(SeqStack& S)
{
	S.top = -1;
	cout << "栈已初始化！" << endl;
}

void PushSeqStack(SeqStack& S,ElemType e)
{
	if(S.top == MAXSIZE -1)
	{
		cout << "该栈已满" << endl;
		exit(1);
	}
	cout << "正在对元素" << e << "进行入栈操作" << endl;
	S.top++;
	S.data[S.top] = e;
}

ElemType PopSeqStack(SeqStack& S)
{
	if(S.top == -1)
	{
		cout << "该栈已空" << endl;
		exit(1);
	}
	ElemType x = S.data[S.top];
	S.top--;
	return x;
}

ElemType TopElemSeqStack(SeqStack& S)
{
	if(S.top == -1)
	{
		cout << "该栈已空" << endl;
		exit(1);
	}
	ElemType top_element = S.data[S.top];
	return top_element;
}

bool EmptySeqStack(SeqStack& S)
{
	if(S.top == MAXSIZE - 1)
		return true;
	else
		return false;
}

bool NoneSeqStack(SeqStack& S)
{
	if(S.top == -1)
		return true;
	else
		return false;
}


