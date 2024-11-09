N=int(input("请输入一个整数N，以计算得到1~N之间满足条件的回文数："))

for num in range(1, N + 1):
    if int(num ** 0.5) ** 2 == num:  # 判断是否为平方数
        # 判断是否为回文数
        num_oct = oct(num)[2:] # 将数字转换为八进制
        if num_oct == num_oct[::-1]: # 判断八进制是否为回文数
            print(num, end=' ')