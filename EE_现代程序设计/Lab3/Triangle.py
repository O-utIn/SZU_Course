n = int(input("请输入一个正整数 n："))

width = len(str(2**(n-1)))  # 计算最大的数字的宽度

for i in range(n):
    # 在每行开头打印空格，使三角形居中对齐
    for k in range(n - i - 1):
        print(" " * width, end="")
    num = 1
    # 打印当前行的数字
    for j in range(i + 1):
        print(str(num).rjust(width), end=" ")
        num = num * (i - j) // (j + 1)
    print()
