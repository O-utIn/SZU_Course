#include <stdio.h>

char * delete_char(char * ps, char ch)
{
    char *p = ps;// �����ַ�������ʼ��ַ
    while (*ps != '\0')
	{
		// �����ǰ�ַ�ΪҪɾ�����ַ�
        if (*ps == ch)
		{
            char *temp = ps;// ���浱ǰλ�õ�ָ��
            // ��������ַ�������ǰ�ƶ�һλ
            while (*temp != '\0')
			{
                *temp = *(temp + 1);
                temp++;
            }
        }
		else
		{
            ps++;// ����������һ���ַ�
        }
    }
    return p; // ���ش�����ַ������׵�ַ
}

int main()
{
    char arr[20] = "ArtgyuAA  bbbtx";
    printf("ԭʼ�ַ�����%s\n", arr);
    char *result = delete_char(arr, 'A');
    printf("�������ַ�����%s\n", result);
    return 0;
}
