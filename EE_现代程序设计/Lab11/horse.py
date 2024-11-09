# 读取输入
N = int(input("请输入需要过河的小马数量:"))
times = list(map(int, input("请输入每匹小马过河需要花费的时间(分钟),正整数之间以一个空格隔开：").split()))

# 将过河时间从小到大排序
times.sort()

# 计算最少需要的时间
total_time = 0
while N > 3:
    # 赶最快的两匹小马过河
    time1 = times[0]
    time2 = times[1]
    total_time += time2
    # 返回
    total_time += time1
    # 赶最慢的两匹小马过河
    time3 = times[N-1]
    time4 = times[N-2]
    total_time += time3
    # 返回
    total_time += time2
    N -= 2

# 处理剩下的1-3匹小马
if N == 2:
    total_time += times[1]
elif N == 3:
    total_time += sum(times)

# 输出结果
print("赶N匹小马全部过河至少需要花费的时间为:",total_time)
