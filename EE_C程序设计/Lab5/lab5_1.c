#include<stdio.h>

int main()
{
	char ch;
	int letter = 0, space = 0, digit = 0, other = 0;
	printf("�������ַ�����ͳ�Ƹ��ַ����и����ַ����������Իس���������:");
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
	printf("Ӣ����ĸ�ĸ���Ϊ:%d\n",letter);
	printf("�ո�ĸ���Ϊ:%d\n",space);
	printf("���ֵĸ���Ϊ:%d\n",digit);
	printf("�����ַ��ĸ���Ϊ:%d\n",other);
	return 0;
}
