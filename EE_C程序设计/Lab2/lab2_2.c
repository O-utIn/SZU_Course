#include <stdio.h>
int main( )
{
	char *name = "王坤";
	char sex = 'M';
	int math=88, chinese=79, English=90;    
	float avg=(math+chinese+English)/3.0; 

	printf("\n");
	printf("这是一个使用 printf 函数按表格输出三个同学信息的程序\n");
	printf("\n");
	printf("姓名	性别	数学	语文	英语	平均\n");
	printf("%-8s%-8c%-8d%-8d%-8d%-8.1f\n", name, sex,math, chinese, English, avg);
	
	name = "李倩";
	sex = 'F';
	math=85, chinese=87, English=92; 
	avg=(math+chinese+English)/3.0; 
	printf("%-8s%-8c%-8d%-8d%-8d%-8.1f\n", name, sex,math, chinese, English, avg);

	name = "顾全";
	sex = 'M';
	math=95, chinese=93, English=98; 
	avg=(math+chinese+English)/3.0; 
	printf("%-8s%-8c%-8d%-8d%-8d%-8.1f\n", name, sex,math, chinese, English, avg);

	return 0;
}

