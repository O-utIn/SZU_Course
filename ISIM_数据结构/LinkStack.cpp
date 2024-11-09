#include <iostream>
using namespace std;

typedef int ElemType;
typedef struct Node
{
	ElemType data;
	struct Node* next;
}*LinkStack;

void InitLinkStack(LinkStack&); //初始化链栈
void PushLinkStack(LinkStack&,ElemType); //入栈
ElemType PopLinkStack(LinkStack&); //出栈
ElemType TopElemLinkStack(LinkStack&); //取栈顶元素
bool NoneLinkStack(LinkStack&); //判断栈空
void Destroy(LinkStack&); //销毁链栈

int main(int argc, char **argv)
{
	LinkStack LS;
	InitLinkStack(LS);

	PushLinkStack(LS,50);
	PushLinkStack(LS,26);
	PushLinkStack(LS,16);
	PushLinkStack(LS,9);

	cout << "正在弹出栈顶元素:" << PopLinkStack(LS) << endl;
	cout << "正在弹出栈顶元素:" << PopLinkStack(LS) << endl;

	cout << "正在查询栈顶元素：" << TopElemLinkStack(LS) << endl;

	cout << "正在判断链栈是否为空(返回1为空，返回0则未空)：" << NoneLinkStack(LS) << endl;

	cout << "正在销毁链栈" << endl;
	Destroy(LS);

	return 0;
}

void InitLinkStack(LinkStack& LStack)
{
	LStack = NULL;
}

void PushLinkStack(LinkStack& LStack,ElemType e)
{
	LinkStack temp = new Node;
	temp -> data = e;
	temp -> next = LStack;
	LStack = temp;
	cout << "正在对元素" << e << "进行入栈操作" << endl;
}

ElemType PopLinkStack(LinkStack& LStack)
{
	if(LStack == NULL)
	{
		cout << "链栈已空" << endl;
		exit(1);
	}
	ElemType x = LStack -> data;
	LinkStack temp = LStack;
	LStack = temp -> next;
	delete temp;
	return x;
}

ElemType TopElemLinkStack(LinkStack& LStack)
{
	if(LStack == NULL)
	{
		cout << "链栈已空，栈顶元素不存在！" << endl;
		exit(1);
	}
	return LStack -> data;
}

bool NoneLinkStack(LinkStack& LStack)
{
	if(LStack == NULL)
		return true;
	else
		return false;
}

void Destroy(LinkStack& LStack)
{
	LinkStack temp;
	while(LStack)
	{
		temp = LStack;
		LStack = LStack -> next;
		delete temp;
	}
	cout << "链栈已被销毁" << endl;
}

