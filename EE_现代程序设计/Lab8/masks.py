def masks(n,quantities):
    quantities.sort(reverse = True) #将口罩数量从大到小排列
    #初始化两个区域的口罩数量
    region1 = 0
    region2 = 0

    for i in range(n):
        if region1 <= region2:
            region1 += quantities[i] #将口罩分配给第一个区域
        else:
            region2 += quantities[i] #将口罩分配给第二个区域

    #按照从大到小的顺序输出结果
    if region1 >= region2:
        print("两个疫情重灾区分得的口罩数量分别为:",region1,region2)
    else:
        print("两个疫情重灾区分得的口罩数量分别为:",region2,region1)

#获取输入，调用函数计算结果并输出
n = int(input("请输入口罩箱数:"))
quantities = list(map(int,input("请输入每箱口罩的数量，以空格分隔:").split()))
masks(n,quantities)
