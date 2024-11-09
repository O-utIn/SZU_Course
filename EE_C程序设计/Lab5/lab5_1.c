#include<stdio.h>

int main()
{
	char ch;
	int letter = 0, space = 0, digit = 0, other = 0;
	printf("请输入字符串以统计该字符串中各种字符的数量，以回车结束输入:");
	while((ch = getchar()) != '\n')
	{
		if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
			letter++;
		else if(ch == ' ')
			space++;
		else if(ch >= '0' && ch <= '9')
			digit++;
		else
			other++;
	} 
	printf("英文字母的个数为:%d\n",letter);
	printf("空格的个数为:%d\n",space);
	printf("数字的个数为:%d\n",digit);
	printf("其它字符的个数为:%d\n",other);
	return 0;
}
