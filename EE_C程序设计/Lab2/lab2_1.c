// �����ܣ���printf�ĸ�ʽ�������������ʽ�����ͬ���͵����� 
#include <stdio.h>
int main( )
{
	char *name = "BAA";   // ������nameָ�������ָ���ַ���"Ҧ��"
	char sex = 'M';	  // 'M'��ʾ�У�'F' ��ʾŮ	
	int math=100, chinese=100, English=99;    

	printf("\n");
	printf("����һ��ʹ�� printf ������������ %s �ɼ��ĳ���\n", name);
	printf("\n");
	//ÿ���ַ��ظ�8�Σ�Ϊ�������λ�����ο� 
	printf("AAAAAAAABBBBBBBBCCCCCCCCDDDDDDDDEEEEEEEE\n");	
	printf("����	�Ա�	��ѧ	����	Ӣ��\n");
//	printf("%-8s%-8c%-8d%-8d%-8d\n", name, sex,math, chinese, English);
	printf("%8s%8c%8d%8d%8d\n", name, sex,math, chinese, English); 
	printf("%10s%3c%5d%8d%6d\n", name, sex,math, chinese, English);
	
	return 0;
}

