#include<stdio.h>
#include<conio.h>

void copyStr(char desStr[],char srcStr[]);

int main()
{
	char Str1[20], Str2[20];
	
	printf("�������һ���ַ���:\n");
	gets(Str1);
	printf("������ڶ����ַ���:\n");
	gets(Str2);
	
	printf("\n����ǰ���ַ���һΪ:%s\n",Str1);
	printf("����ǰ���ַ�����Ϊ:%s\n",Str2);
	copyStr(Str1,Str2);
	printf("\n��������ַ���һΪ:%s\n",Str1);
	printf("��������ַ�����Ϊ:%s\n",Str2);
	
	return 0;
}

void copyStr(char desStr[],char srcStr[])
{
	int i = 0;
	while(srcStr[i] != '\0')
	{
		desStr[i] = srcStr[i];
		i++;
	}
	desStr[i] = '\0';
}
