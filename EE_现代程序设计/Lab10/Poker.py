def flip(N):
    if N < 4:
        return -1
    if N == 5:
        return 3
    if N % 3 == 0:
        # 对于3的倍数张扑克牌，需要翻转的次数为 N//3
        return N // 3
    else:
        # 其他张数扑克牌，需要翻转的次数为 N//2 + 2
        return N // 2 + 2

N = int(input("请输入需要翻转的扑克牌数量:"))
result = flip(N)
print("至少需要翻转的次数为:",result)
