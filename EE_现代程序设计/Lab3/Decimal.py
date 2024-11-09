a = int(input("请输入分子 a（0 < a < b < 100）："))
b = int(input("请输入分母 b（0 < a < b < 100）："))
n = int(input("请输入要求的小数点后第 n 位（1 <= n <= 10000）："))

if 0<a<b<100 and 1<=n<= 10000:
    # 使用长除法计算小数部分
    decimal = ""
    remainder = a % b

    for i in range(n):
        remainder *= 10 # 将余数乘以10，相当于将小数点向右移动一位
        decimal += str(remainder // b) # 将商（余数除以分母）的整数部分转换为字符串，并添加到小数部分的字符串中
        remainder %= b
    print(f"小数点后第 {n} 位的数字是：{decimal[-1]}")
else:
    print("您输入的数字有误，请重启程序按照要求输入正确数字")