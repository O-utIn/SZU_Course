// 程序功能：用printf的格式控制来按表格形式输出不同类型的数据 
#include <stdio.h>
int main( )
{
	char *name = "BAA";   // 定义了name指针变量，指向字符串"姚明"
	char sex = 'M';	  // 'M'表示男，'F' 表示女	
	int math=100, chinese=100, English=99;    

	printf("\n");
	printf("这是一个使用 printf 函数按表格输出 %s 成绩的程序\n", name);
	printf("\n");
	//每个字符重复8次，为后面输出位置做参考 
	printf("AAAAAAAABBBBBBBBCCCCCCCCDDDDDDDDEEEEEEEE\n");	
	printf("姓名	性别	数学	语文	英语\n");
//	printf("%-8s%-8c%-8d%-8d%-8d\n", name, sex,math, chinese, English);
	printf("%8s%8c%8d%8d%8d\n", name, sex,math, chinese, English); 
	printf("%10s%3c%5d%8d%6d\n", name, sex,math, chinese, English);
	
	return 0;
}

