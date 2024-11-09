def hex_triangle(n,h):
    hex_num = int(n,16) #将输入的十六进制转换为十进制
    #循环h次，生成h行三角形
    for i in range(h):
        row = [] #设置一个行列表用于保存每行数据
        #每行的数量从1到i+1
        for j in range(i + 1):
            num = hex(hex_num)[2:].upper() #将当前十进制数转换为十六进制，并使用大写字母
            row.append(num) #将当前的十六进制数添加到行列表中
            hex_num += h #按照规则，下一个数比上一个数大h
        print("  ".join(row)) #输出当前行

#获取输入并调用函数输出结果
n,h = input("请输入一个十六进制整数n和一个十进制整数h，空格分隔数据:\n").split()
print("计算得出的十六进制直角三角形为:")
hex_triangle(n,int(h))
