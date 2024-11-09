#include <bits/stdc++.h>
using namespace std;

typedef char ElemType;
enum flag{Child,Thread};
typedef struct BiNode
{
	ElemType data;
	BiNode *left,*right;
	flag ltype,rtype;
}*Bitree;

void Create(Bitree& t) //先序创建二叉树
{ 
	static int i =0;
	char input[] = {'a','b','c','#','d','#','#','#','e','f','#','#','g','h','#','#','i','#','#'};
	char ch = input[i];
	i++;
	
	if(ch == '#')
    {
        //如果输入的是#，则二叉树的节点为空节点
		t=NULL;
		return;
	}
	else  
        //如果输入不是#，则分配节点空间
		if(!(t = new BiNode))
        {
			cout << "分配失败" << endl;
			return;
		}
        else
        {
            //分配空间成功
			t -> data = ch;

			//先将左右指针类型设为孩子类型
			t -> ltype = Child;
			t -> rtype = Child;

			Create(t -> left);
			Create(t -> right);
		}
}

void PreOrder(Bitree t) //先序遍历
{
	if(t != NULL)
    {
        cout << t -> data << " ";
		PreOrder(t -> left);
		PreOrder(t -> right);
	}
}

void InOrder(Bitree t) //中序遍历
{
   
	if(t != NULL)
    {
        InOrder(t -> left);
        cout << t -> data << " ";        
        InOrder(t -> right);
    }
}
void PostOrder(Bitree t) //后序遍历
{   
    if(t != NULL)
    {
        PostOrder(t -> left);    
        PostOrder(t -> right);
        cout << t -> data << " ";    
    }
}

void InThreaded(Bitree &t) //中序线索化
{
	//prenode变量设置为静态变量，确保每次递归正确
	static Bitree prenode = NULL;
	if(t)
    {
		InThreaded(t -> left); //处理左子树

		//处理根节点
		if(!t -> left)
        {
			t -> ltype = Thread;
			t -> left = prenode;
		}

		if(prenode && !prenode -> right)
        {
			prenode -> rtype = Thread;
			prenode -> right = t;
		}
		prenode = t;
		
	    InThreaded(t -> right); //处理右子树	
	}	
}


BiNode* InOrder_Post(BiNode *p) //线索化后查找某节点的后继
{
	BiNode *q;
	if(p -> rtype == Thread)
		return p -> right;
	else
    {
		q = p -> right;
		while(q -> ltype == Child)
			q = q -> left;
		return q;
	}
}

BiNode* InOrder_Pre(BiNode *p) //线索化后查找某节点的前驱
{
	BiNode *q;
	if(p -> ltype == Thread)
		return p -> left;
	else
    {
		q = p -> left;
		while(q -> rtype == Child)
			q = q -> right;
		return q;
	}
}

void DestroyTree(Bitree t) //销毁二叉树
{
	if(t)
    {
		if(t -> ltype == Child)
			DestroyTree(t -> left);
		if(t -> rtype = Child)
			DestroyTree(t -> right);
		delete t;
		t = NULL;
	}
}

int main(int argc, char **argv)
{
	Bitree tree = NULL;
	Create(tree);

    cout << endl << "先序遍历" << endl;
	PreOrder(tree);
	cout << endl << "中序遍历" << endl;
	InOrder(tree);
	cout << endl << "后序遍历" << endl;
	PostOrder(tree);

	InThreaded(tree);
	BiNode *temp=InOrder_Pre(tree);
	cout << endl << "根节点的前驱是" << temp -> data << endl;
	temp=InOrder_Post(tree);
	cout << "根节点的后继是" << temp -> data << endl;

	DestroyTree(tree);
	
	return 0;
}

