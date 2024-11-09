#include <stdio.h>

char * delete_char(char * ps, char ch)
{
    char *p = ps;// 保存字符串的起始地址
    while (*ps != '\0')
	{
		// 如果当前字符为要删除的字符
        if (*ps == ch)
		{
            char *temp = ps;// 保存当前位置的指针
            // 将后面的字符依次向前移动一位
            while (*temp != '\0')
			{
                *temp = *(temp + 1);
                temp++;
            }
        }
		else
		{
            ps++;// 继续遍历下一个字符
        }
    }
    return p; // 返回处理后字符串的首地址
}

int main()
{
    char arr[20] = "ArtgyuAA  bbbtx";
    printf("原始字符串：%s\n", arr);
    char *result = delete_char(arr, 'A');
    printf("处理后的字符串：%s\n", result);
    return 0;
}
