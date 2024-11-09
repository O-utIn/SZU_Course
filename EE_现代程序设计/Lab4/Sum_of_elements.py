n = int (input('请输入一个正整数 n (1<= n <=100):'))
getchar = input('请输入{}个大小范围在1~10之间的数字，相邻两个数字之间用空格隔开：'.format(n))
#将输入的字符串保存到列表中
lst = getchar.split (' ')
list =[int(i) for i in lst]
#筛选出列表中只出现一次的元素
num =[]
for i in list:
    count = list.count ( i )
    if count ==1:
        num.append ( i )

#求和并打印
sum =0
for j in num:
    sum = sum + j 
print("列表中所有只出现一次的元素之和为{}".format(sum))
