#include <stdio.h>


int main( )
{
	char capital, ch;
	
	printf("������һ����д��ĸ��"); 
	
	capital = getchar();
	putchar('\n');
	
	ch = 'A' + (capital - 'A' + 13) % 26;

	printf("�ô�д��ĸ�ĶԽ���ĸΪ��");
	putchar(ch);

	return 0;
}

