#include <iostream>
using namespace std;

typedef int ElemType;
typedef struct Node
{
	ElemType data;
	struct Node *next;
}Node;
typedef struct
{
	Node *front;
	Node *rear;
}LinkQueue;

void InitLQueue(LinkQueue&); //初始化
void EnLQueue(LinkQueue&,ElemType); //入队
ElemType DeLQueue(LinkQueue&); //出队
bool LQueueEmpty(LinkQueue&); //判断队空
void DestroyListQueue(LinkQueue&); //销毁队列

int main(int argc, char **argv)
{
	LinkQueue L;
	InitLQueue(L);
	EnLQueue(L,11);
	EnLQueue(L,22);
	EnLQueue(L,33);
	DeLQueue(L);
	cout << "队列是否已空（返回1则已空，返回0则未空）：" << LQueueEmpty(L) << endl;
	DestroyListQueue(L);
	return 0;
}

void InitLQueue(LinkQueue& L)
{
	L.front = L.rear = new Node;
	L.front -> next = NULL;
}

void EnLQueue(LinkQueue& L,ElemType e)
{
	Node *temp = new Node;
	temp -> data = e;
	temp -> next = NULL;
	L.rear -> next = temp;
	L.rear = temp;
	cout << "正在入队的元素是:" << e << endl;
}

ElemType DeLQueue(LinkQueue& L)
{
	if(L.front == L.rear)
	{
		cout << "队列已空" << endl;
		exit(1);
	}
	Node *temp = L.front -> next;
	ElemType x = temp -> data;
	cout << "正在出队的元素是：" << x << endl;
	L.front = temp -> next;
	if(L.rear == temp)
		L.rear = L.front;
	delete temp;
}

bool LQueueEmpty(LinkQueue &L)
{
	return L.front == L.rear;
}

void DestroyListQueue(LinkQueue &L)
{
    while(L.front)
	{
	  L.rear=L.front->next;
	  delete L.front;
	  L.front=L.rear;
	}
	cout << "队列已销毁" << endl;
}
