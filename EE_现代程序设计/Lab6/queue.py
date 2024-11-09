n = int(input("请输入学生的数量n(1<=n<=100)：")) #学生的数量
m = int(input("请输入口令的数量m(1<=m<=100)：")) #口令的数量
commands = list(map(int,input("请输入各口令的值，以空格分隔数据:").split())) #口令的x值列表

#初始化数据，使所有学生都面向老师
students = [True] * n


for command in commands:
    #对于口令x的倍数
    for i in range(command - 1,n + 1,command):
        students[i - 1] ^= True   #转身

#计算最终面向老师的学生数量
count = students.count(True)
print("最终面向老师的学生数量为:%d"%count)
