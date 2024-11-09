n = int(input("请输入一个正整数 n："))

# 设置计数器，记录已找到的素数的数量
count = 0
# 从2开始判断
num = 2

while count < n:
    for i in range(2, int(num ** 0.5) + 1):  # 遍历2到num的平方根之间的数字
        if num % i == 0:  # 如果num能够整除i，说明num不是素数
            break
    else:
        print(num)  # 输出当前找到的素数
        count += 1  # 计数器加1，表示已找到一个素数
    num += 1  # 继续判断下一个数字
