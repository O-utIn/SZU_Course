def int_to_roman(num):
    # 定义罗马数字的符号和对应的数值
    symbols = ["M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"]
    values = [1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1]

    roman = ""  # 用于存储转换后的罗马数字

    # 遍历符号和数值列表
    for i in range(len(symbols)):
        while num >= values[i]:  # 当输入的数值大于等于当前数值时，将对应的符号添加到结果字符串中
            roman += symbols[i]
            num -= values[i]  # 减去对应的数值

    return roman

# 读取输入的正整数
num = int(input('请输入一个正整数:'))

# 转换为罗马数字
roman_num = int_to_roman(num)

# 输出转换后的罗马数字
print(roman_num)
