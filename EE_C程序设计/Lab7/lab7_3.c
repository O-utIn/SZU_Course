#include <stdio.h>
#include <string.h>

// ��ת�ַ������ӵ�λ��ʼ���� 
void reverseStr(char* str)
{
    int length = strlen(str);
    int i;
    for (i = 0; i < length / 2; i++)
	{
        char temp = *(str + i);
        *(str + i) = *(str + length - i - 1);
        *(str + length - i - 1) = temp;
    }
}

// ���������
void subtract(char* num1, char* num2, char* result)
{
	// ��ת��������
    reverseStr(num1);
    reverseStr(num2);
    
    int borrow = 0; // ��λ��ʶ 
    int i = 0;
    while (*(num2 + i) != '\0')
	{
        int digit1 = (i < strlen(num1)) ? (*(num1 + i) - '0') : 0; // ��ȡ��һ�����ĵ�ǰλ����
        int digit2 = *(num2 + i) - '0'; // ��ȡ�ڶ������ĵ�ǰλ����
        int diff = digit1 - digit2 - borrow; // �����ֵ
        if (diff < 0)
		{
            diff += 10; // �����ֵΪ����������10
            borrow = 1; // ���ý�λΪ1
        } 
		else
		{
            borrow = 0; // �����λΪ0
        }
        *(result + i) = diff + '0'; // ����ֵת��Ϊ�ַ�������������
        i++;
    }
    
    while (i < strlen(num1))
	{
        int digit = *(num1 + i) - '0' - borrow; // ����ʣ��ĵ�һ������λ
        if (digit < 0)
		{
            digit += 10;
            borrow = 1;
        }
		else
		{
            borrow = 0;
        }
        *(result + i) = digit + '0';
        i++;
    }
    
    *(result + i) = '\0'; // �������ĩβ�����ַ���������
    reverseStr(result); // ��ת�������õ����ս��
}

int main()
{
    char num1[100];
    char num2[100];
    char result[100];
    
    printf("�������һ����������Ϊ������: ");
    scanf("%s", num1);
    printf("����ڶ�����������Ϊ����: ");
    scanf("%s", num2);
    
    subtract(num1, num2, result);
    
    printf("Result: %s\n", result);
    
    return 0;
}
