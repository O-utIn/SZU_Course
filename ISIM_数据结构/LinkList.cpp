#include <iostream>
using namespace std;

typedef int Elem;
typedef struct Node
{
	Elem data;
	struct Node* next;
}Node,*LinkList;

void Init_Link(LinkList&); //初始化链表
void Creat_Link(LinkList&, Elem datalist[], int); //创建链表
void Show_Link(LinkList&); //遍历输出链表
int Length_Link(LinkList&); //查询链表长度
int Locat_Elem(LinkList&, Elem); //按值查找元素
void Insert_Elem(LinkList&, Elem, int); //插入新元素
void Delete_Index(LinkList&, int); //按索引删除元素
void Destroy_Link(LinkList&); //销毁链表
void UpsideDown_Link(LinkList&); //逆转链表

int main(int argc, char **argv)
{
	LinkList A;
	Init_Link(A);

	Elem Data[50] = {10,2,36,55,7,20,31,1,5};
	int n;

	cout << "请输入你需要的链表长度:";
	cin >> n;
	Creat_Link(A,Data,n);


	Show_Link(A);
	cout << endl;


	cout << "链表长度为:" << Length_Link(A) << endl;
	cout << endl;


	Elem e;
	cout << "请输入你想要查找的元素值:";
	cin >> e;
	cout << "该元素所在链表中的位置为:" << Locat_Elem(A,e) << endl;
	cout << endl;


	cout << "请输入你想要插入的元素值:";
	cin >> e;
	cout << "请输入你想要在链表中哪个位置插入该元素:";
	cin >> n;
	Insert_Elem(A,e,n);
	cout << endl;


	cout << "请输入你想要删除的元素的索引值:";
	cin >> n;
	Delete_Index(A,n);
	cout << endl;


	char button;
	cout << "如果你想要销毁链表，输入'y' or 'Y';否则输入'n' or 'N'";
	cin >> button;
	if(button == 'Y')
		Destroy_Link(A);
	else
		Show_Link(A);
	cout << endl;


	UpsideDown_Link(A);

	return 0;
}


void Init_Link(LinkList& L)
{
	L = new Node;
	L -> next = NULL;
	cout << "链表已初始化！" << endl;
}


void Creat_Link(LinkList& L, Elem datalist[], int n)
{
	LinkList temp;
	L = new Node;
	L -> next = NULL;
	for(int i = n - 1; i >= 0; i--)
	{
		temp = new Node;
		temp -> data = datalist[i];
		temp -> next = L -> next;
		L -> next = temp;
	}
}

void Show_Link(LinkList& L)
{
	LinkList temp = L -> next;
	cout << "链表为:";
	while(temp)
	{
		cout << temp -> data << " ";
		temp = temp -> next;
	}
	cout << endl;
}

int Length_Link(LinkList& L)
{
	LinkList temp = L -> next;
	int len = 0;
	while(temp)
	{
		len++;
		temp = temp -> next;
	}
	return len;
}

int Locat_Elem(LinkList& L, Elem element)
{
	LinkList temp = L -> next;
	int index = 1;
	while(temp && temp -> data != element)
	{
		temp = temp -> next;
		index++;
	}
	if(temp)
		return index;
	else
		return 0;
}

void Insert_Elem(LinkList& L, Elem element, int index)
{
	LinkList temp = L;
	LinkList s;
	for(int i = 1;i <= index - 1;i++)
		temp = temp -> next;
	s = new Node;
	s -> data = element;
	s -> next = temp ->	next;
	temp -> next = s;
	cout << "插入操作已完成!";
	Show_Link(L);
}

void Delete_Index(LinkList& L, int index)
{
	LinkList temp = L;
	LinkList qtemp;
	if(index > Length_Link(L) || index <= 0)
	{
		cout << "该输入索引非法!" << endl;
		exit(1);
	}
	int i = 1;
	while(temp && i <= index - 1)
	{
		temp = temp -> next;
		i++;
	}
	qtemp = temp -> next;
	temp -> next = qtemp -> next;
	delete qtemp;
	cout << "删除操作已完成!";
	Show_Link(L);
}

void Destroy_Link(LinkList& L)
{
	LinkList temp = L;
	LinkList qtemp;
	while(temp)
	{
		qtemp = temp;
		temp = temp -> next;
		delete qtemp;
	}
	cout << "链表已被销毁!" << endl;
}

void UpsideDown_Link(LinkList& L)
{
	LinkList temp,s;
	temp = L -> next;
	L -> next = NULL;
	while(temp)
	{
		s = temp;
		temp = temp -> next;
		s -> next = L -> next;
		L -> next = s;
	}
	cout << "链表已被逆置!";
	Show_Link(L);
}