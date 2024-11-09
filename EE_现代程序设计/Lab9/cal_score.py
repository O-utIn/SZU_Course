n = int(input("请输入学校总数量:")) #参加运动会的学校总数

#用于记录每个学校的编号和奖牌数
medal_counts = []

#输入每个学校的编号和奖牌数
for i in range(n):
    school,medal_count = map(int,input("请输入第{}组数据，以空格分隔学校编号和奖牌数:".format(i+1)).split())
    medal_counts.append((school,medal_count))

#按奖牌数由高到低进行排序
medal_counts.sort(key = lambda x:(-x[1],x[0]))

#输出结果
print("按照奖牌数由高到低进行排序后的结果为:")
for school,medal_count in medal_counts:
    print(school,medal_count)
