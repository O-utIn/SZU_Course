#include <iostream>
using namespace std;

const int MAXSIZE = 100;
typedef int ElemType;

typedef struct Node
{
    int i, j;
    ElemType e;
    Node *right, *down;
}Node;

typedef struct
{
    int rows, cols, nums;
    Node *head;
}SparseMatrix;

void InitMatrix(SparseMatrix&);
void Output(SparseMatrix);

int main(int argc, char **argv)
{
    SparseMatrix A;
    InitMatrix(A);
    Output(A);
    return 0;
}

void InitMatrix(SparseMatrix& S)
{
    cout << "请输入矩阵的行数、列数，以及非零元素的个数（rows cols nums）：";
    cin >> S.rows >> S.cols >> S.nums;

    S.head = new Node;
    S.head -> right = S.head;
    S.head -> down = S.head;
    Node *temp = S.head;
    
    for (int n = 0; n < S.nums; n++)
    {
        Node *newnode = new Node;
        cout << "请输入该元素的下标位置及元素值（i j value）：";
        cin >> newnode -> i >> newnode -> j >> newnode -> e;

        newnode -> right = temp -> right;
        temp -> right = newnode;
        newnode -> down = temp -> down;
        temp -> down = newnode;
        temp = newnode;
    }
}

void Output(SparseMatrix S)
{
    cout << "该矩阵为:" << endl;
    Node *temp = S.head -> right;
    for (int i = 1; i <= S.rows; i++)
    {
        for (int j = 1; j <= S.cols; j++)
        {
            while (temp != S.head)
            {
                    if (i == temp -> i && j == temp -> j)
                {
                    cout << temp -> e << "\t";
                    break;
                }
                temp = temp -> right;
            }
            if (temp == S.head)
                cout << "0\t";

            temp = S.head -> right;
        }
        cout << endl;
    }
}