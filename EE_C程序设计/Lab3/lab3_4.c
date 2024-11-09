#include <stdio.h>


int main( )
{
	char capital, ch;
	
	printf("请输入一个大写字母："); 
	
	capital = getchar();
	putchar('\n');
	
	ch = 'A' + (capital - 'A' + 13) % 26;

	printf("该大写字母的对角字母为：");
	putchar(ch);

	return 0;
}

