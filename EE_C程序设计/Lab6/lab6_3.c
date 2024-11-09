#include<stdio.h>
#include<conio.h>

void copyStr(char desStr[],char srcStr[]);

int main()
{
	char Str1[20], Str2[20];
	
	printf("请输入第一个字符串:\n");
	gets(Str1);
	printf("请输入第二个字符串:\n");
	gets(Str2);
	
	printf("\n拷贝前的字符串一为:%s\n",Str1);
	printf("拷贝前的字符串二为:%s\n",Str2);
	copyStr(Str1,Str2);
	printf("\n拷贝后的字符串一为:%s\n",Str1);
	printf("拷贝后的字符串二为:%s\n",Str2);
	
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
