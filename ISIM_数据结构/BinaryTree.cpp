#include <bits/stdc++.h>
using namespace std;

typedef char ElemType;
typedef struct BiNode
{
	ElemType data;
	BiNode *left,*right;
}*Bitree;

void Create(Bitree& T) //先序创建二叉树
{
	char ch;
	cin>>ch;
	if(ch == '#')
        //如果输入的是#，则二叉树的节点为空节点
		T = NULL;
	else
	{
		T = new BiNode;
		T -> data = ch;
		Create(T -> left);
		Create(T -> right);
	}
}

void PreOrder(Bitree T) //先序遍历
{
	if(T != NULL){
        cout << T -> data << " ";
		PreOrder(T -> left);
		PreOrder(T -> right);
	}
}

void InOrder(Bitree T) //中序遍历
{
	if(T != NULL)
    {
        InOrder(T -> left);
        cout << T -> data << " ";         
        InOrder(T -> right);
    }
}

void PostOrder(Bitree T) //后序遍历
{
    if(T != NULL)
    {
        PostOrder(T -> left);    
        PostOrder(T -> right);
        cout << T -> data <<" ";    
    }
}

int main(int argc, char **argv)
{
	Bitree T = NULL; //初始化二叉树
	Create(T);
	
	cout << endl << "先序遍历结果：" << endl;
	PreOrder(T);

	cout << endl << "中序遍历结果：" << endl;
	InOrder(T);

	cout << endl << "后序遍历结果：" << endl;
	PostOrder(T);
	
	return 0;
}

