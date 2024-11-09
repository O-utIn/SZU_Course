N = int(input('请输入记录的年数N(0<N<1000000)：')) #数据记录的年数
data = list(map(int,input('请输入各年最大数量，各数据之间以空格分隔(0<正整数<=1*10e9)：').split())) #获取每年数据

max_len = 0 #用于计算最长连续年数
current_len = 1 #用于记录当年连续年数

#遍历各年数据进行计算
for i in range(1,N):
    if data[i] > data[i-1]: #当增长时，计数加一
        current_len += 1
    else:
        max_len = max(max_len,current_len) #更新最大连续年数以及重置当前连续年数的值
        current_len = 1

#计算最终的最大连续年数值
max_len = max(max_len,current_len)
print("最大数量连续增加记录中的最长连续年数为:%d"%max_len)
