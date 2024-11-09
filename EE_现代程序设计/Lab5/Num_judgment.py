#用户输入各数字
num = input('请输入数字，以逗号分隔，其中有一个数字只出现一次，其他数字均出现两次:')
num = num.split (',')

#使用字典统计
dict ={}

#将数字记录到字典中并初始化次数
for number in num :
    dict[number]= dict.get(number,0) + 1

#判断只出现一次的数字
for number, count in dict.items():
    if count ==1:
        print(number)
        break