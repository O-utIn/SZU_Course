#include <iostream>
using namespace std;

const int MAXSIZE = 100;
typedef int ElemType;
typedef struct
{
	ElemType data[MAXSIZE];
	int front;
	int rear;
}SeqQueue;

void InitSeqQueue(SeqQueue&); //初始化队列
void EnSQueue(SeqQueue&,ElemType); //入队
ElemType DeSQueue(SeqQueue&); //出队
void ShowSQueue(SeqQueue&); //遍历输出队列
bool QueueEmpty(SeqQueue&); //判断队列是否为空
bool QueueFull(SeqQueue&); //判断队列是否已满

int main(int argc, char **argv)
{
	SeqQueue S;
	InitSeqQueue(S);
	EnSQueue(S,11);
	EnSQueue(S,22);
	EnSQueue(S,33);
	ShowSQueue(S);
	DeSQueue(S);
	DeSQueue(S);
	DeSQueue(S);
	cout << "队列是否为空（输出1为空，输出0则未空）：" << QueueEmpty(S) << endl;
	cout << "队列是否已满（输出1已满，输出0则未满）：" << QueueFull(S) << endl;
	return 0;
}

void InitSeqQueue(SeqQueue& S)
{
	S.front = 0;
	S.rear = 0;
}

void EnSQueue(SeqQueue& S,ElemType e)
{
	if((S.rear+1)%MAXSIZE == S.front)
	{
		cout << "队列已满" << endl;
		exit(1);
	}
	cout << "正在入队的元素：" << e << endl;
	S.rear = (S.rear + 1) % MAXSIZE;
	S.data[S.rear] = e;
}

ElemType DeSQueue(SeqQueue& S)
{
	if(S.front == S.rear)
	{
		cout << "队列已空" << endl;
		exit(1);
	}
	S.front = (S.front + 1) % MAXSIZE;
	ElemType x = S.data[S.front];
	cout << "出队的元素是：" << x << endl;
	return x;
}

void ShowSQueue(SeqQueue& S)
{
	if(S.front == S.rear)
	{
		cout << "None" << endl;
		exit(1);
	}
	cout << "队列为:";
	for(int i = S.front + 1;i <= S.rear;i++)
		cout << S.data[i] << " ";
	cout << endl;
}

bool QueueEmpty(SeqQueue &S)
{
	return S.front == S.rear;
}

bool QueueFull(SeqQueue &S)
{
	return (S.rear + 1) % MAXSIZE == S.front;
}
