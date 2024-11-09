#include <stdio.h>
#include <string.h>

// 反转字符串，从低位开始计算 
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

// 大整数相减
void subtract(char* num1, char* num2, char* result)
{
	// 反转第两个数
    reverseStr(num1);
    reverseStr(num2);
    
    int borrow = 0; // 借位标识 
    int i = 0;
    while (*(num2 + i) != '\0')
	{
        int digit1 = (i < strlen(num1)) ? (*(num1 + i) - '0') : 0; // 获取第一个数的当前位数字
        int digit2 = *(num2 + i) - '0'; // 获取第二个数的当前位数字
        int diff = digit1 - digit2 - borrow; // 计算差值
        if (diff < 0)
		{
            diff += 10; // 如果差值为负数，加上10
            borrow = 1; // 设置借位为1
        } 
		else
		{
            borrow = 0; // 否则借位为0
        }
        *(result + i) = diff + '0'; // 将差值转换为字符并存入结果数组
        i++;
    }
    
    while (i < strlen(num1))
	{
        int digit = *(num1 + i) - '0' - borrow; // 处理剩余的第一个数的位
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
    
    *(result + i) = '\0'; // 结果数组末尾加上字符串结束符
    reverseStr(result); // 反转结果数组得到最终结果
}

int main()
{
    char num1[100];
    char num2[100];
    char result[100];
    
    printf("请输入第一个大整数作为被减数: ");
    scanf("%s", num1);
    printf("输入第二个大整数作为减数: ");
    scanf("%s", num2);
    
    subtract(num1, num2, result);
    
    printf("Result: %s\n", result);
    
    return 0;
}
