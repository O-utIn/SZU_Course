#include <stdio.h>
int main( )
{
	char *name = "����";
	char sex = 'M';
	int math=88, chinese=79, English=90;    
	float avg=(math+chinese+English)/3.0; 

	printf("\n");
	printf("����һ��ʹ�� printf ����������������ͬѧ��Ϣ�ĳ���\n");
	printf("\n");
	printf("����	�Ա�	��ѧ	����	Ӣ��	ƽ��\n");
	printf("%-8s%-8c%-8d%-8d%-8d%-8.1f\n", name, sex,math, chinese, English, avg);
	
	name = "��ٻ";
	sex = 'F';
	math=85, chinese=87, English=92; 
	avg=(math+chinese+English)/3.0; 
	printf("%-8s%-8c%-8d%-8d%-8d%-8.1f\n", name, sex,math, chinese, English, avg);

	name = "��ȫ";
	sex = 'M';
	math=95, chinese=93, English=98; 
	avg=(math+chinese+English)/3.0; 
	printf("%-8s%-8c%-8d%-8d%-8d%-8.1f\n", name, sex,math, chinese, English, avg);

	return 0;
}

