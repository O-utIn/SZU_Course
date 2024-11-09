n = int(input("请输入蛇的数量n (2<=n<=20，且n为偶数): "))
lengths = list(map(int, input("请输入{}个正整数，表示每条蛇的长度，相邻两个数字之间用空格隔开: ".format(n)).split()))

beans = 0
#排序
lengths.sort()

#对每相邻两个进行计算
for i in range(1, n, 2):
    beans += lengths[i] - lengths[i-1]

print("这些蛇要两两组队参赛，至少要吃{}个金豆。".format(beans))
