
#include <iostream>
using namespace std;

const int MAXSIZE = 100;
typedef int ElemType;

typedef struct
{
	int i,j;
	ElemType e;
}TrNode;

typedef struct
{
	int rows,cols,nums;
	TrNode data[MAXSIZE];
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
	cout << "�����������������������Լ�����Ԫ�صĸ�����rows cols nums����";
	cin >> S.rows >> S.cols >> S.nums;
	for(int n = 0;n < S.nums;n++)
	{
		new TrNode;
		cout << "�������Ԫ�ص��±�λ�ü�Ԫ��ֵ��i j value����";
		cin >> S.data[n].i >> S.data[n].j >> S.data[n].e;
	}
}

void Output(SparseMatrix S)
{
	cout << "�þ���Ϊ:" << endl;
	int n;
	for(int i = 1;i <= S.rows;i++)
	{
		for(int j = 1;j <= S.cols;j++)
		{
			for(n = 0;n < S.nums;n++)
			{
				if(i == S.data[n].i && j == S.data[n].j)
					break;
			}
			if(n < S.nums)
				cout << S.data[n].e << "\t";
			else
				cout << "0\t";
		}
		cout << endl;
	}

}
