#定义一个函数用于计算调整糖果的次数
def adjust(candies):
    #统计有几个小朋友、糖果的总数量以及糖果的平均数量
    n = len(candies)
    total = sum(candies)
    avg = total // n

    #初始化次数为0
    count = 0
    
    #从第一个到倒数第二个小朋友
    for i in range(n - 1):
        #如果该小朋友的糖果大于平均值，则将其多出平均值的糖果分给右边的小朋友
        if candies[i] > avg:
            diff = candies[i] - avg
            candies[i] -= diff
            candies[i + 1] += diff
            count +=1

        #如果该小朋友的糖果小于平均值，则将从右边的小朋友手中获得糖果以达到平均值
        elif candies[i] < avg:
            diff = avg - candies[i]
            candies[i] += diff
            candies[i + 1] -= diff
            count += 1

    return count


#读取输入
input_str = input()
candies = list(map(int, input_str.split(',')))

#调用函数计算结果并输出
result = adjust(candies)
print(result)
