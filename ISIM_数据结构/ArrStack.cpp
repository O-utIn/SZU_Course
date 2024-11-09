#include <iostream>
using namespace std;

const int MAXSIZE = 5; //设置数组空间大小
typedef int ElemType;
typedef struct
{
    int arr[MAXSIZE];  //初始化一个固定大小的数组，最大容量为MAXSIZE
    int size;  //用于初始化数组大小
    int top1;  //栈1的栈顶指针
    int top2;  //栈2的栈顶指针
}Stack;

void Push1(Stack&,ElemType);
void Push2(Stack&,ElemType);
void Show(Stack&);
bool FullOrNot(Stack&);
ElemType Pop1(Stack&);
ElemType Pop2(Stack&);

int main()
{
    Stack S;
    S.size = MAXSIZE; //初始化数组的最大容量
    S.top1 = -1; //第一个栈的栈顶指针
    S.top2 = S.size; //第二个栈的栈顶指针

    Push1(S,1);
    Push2(S,5);
    Push2(S,9);
    Push2(S,3);
    Push1(S,88);
    Push1(S,19);

    Show(S);

    cout << "该数组是否已满（0代表未满；1代表已满）：" << FullOrNot(S) << endl;

    cout << Pop1(S) << endl;

    Push2(S,65);
    Show(S);

    return 0;
}

void Push1(Stack& S,ElemType data)
{
    //如果栈1的[栈顶指针]小于栈2的[[栈顶指针]-1]，说明数组还有空间
    if(S.top1 < S.top2 - 1)
    {
        cout << "正在对栈1进行入栈操作，入栈元素为：" << data << endl;
        S.arr[++S.top1] = data;  //将数据压入栈1，并更新栈1的顶指针
    }
    else
        cout << "空间已满，无法继续进行入栈操作！" << endl;
}

void Push2(Stack& S,ElemType data)
{
    //如果栈2的[栈顶指针]大于栈1的[[栈顶指针]+1]，说明数组还有空间
    if(S.top2 > S.top1 + 1)
    {
        cout << "正在对栈2进行入栈操作，入栈元素为：" << data << endl;
        S.arr[--S.top2] = data;  //将数据压入栈2，并更新栈2的顶指针
    }
    else
        cout << "空间已满，无法继续进行入栈操作！" << endl;
}

ElemType Pop1(Stack& S)
{
    //如果栈1的[栈顶指针]不为-1，说明栈1不为空，即可进行出栈操作
    if(S.top1 >= 0)
    {
        cout << "正在对栈1进行出栈操作，出栈元素为："; 
        return S.arr[S.top1--];  //弹出栈1的栈顶元素，并更新栈顶指针
    }
    else
    {
        cout << "栈1已空,无法进行出栈操作！" << endl;
        return -1;
    }
}

ElemType Pop2(Stack& S)
{
    //如果栈2的栈顶指针小于[[数组大小]-1]，说明栈2不为空，即可进行出栈操作
    if(S.top2 <= S.size - 1)
    {
        cout << "正在对栈1进行出栈操作，出栈元素为："; 
        return S.arr[S.top2++];  //弹出栈2的栈顶元素，并更新栈顶指针
    }
    else
    {
        cout << "栈2已空,无法进行出栈操作！" << endl;
        return -1;
    }
}


void Show(Stack& S)
{
    cout << "栈1：";
    for(int i = 0;i <= S.top1; i++)
        cout << S.arr[i] << " ";
    cout << "\n栈2：";
    for(int j = S.size - 1;j >= S.top2;j--)
        cout << S.arr[j] << " ";
    cout << endl;
}

bool FullOrNot(Stack& S)
{
    // 如果栈1的[[栈顶指针]+1]等于栈 2 的[栈顶指针]，说明栈已满
    return S.top1 + 1 == S.top2;
}