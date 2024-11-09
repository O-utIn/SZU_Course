#获取用户输入的分子和分母，并对数据进行处理
up,down = input('请输入分子和分母，用空格隔开：').split(' ')
up = int(up)
down = int(down)

#整数部分
decimal = str(up // down) + "."

#计算余数并记录出现的位置
remainder = up % down
dict ={}

while remainder != 0:
    if remainder in dict :#出现循环部分
        #在小数字符串中插入括号将循环部分放在括号内
        decimal = decimal [:dict[remainder]] + "(" + decimal[dict[remainder]:] + ")"
        break
    #记录当前余数的位置
    dict[remainder] = len(decimal)
    remainder *=10
    #将商添加到小数字符串中
    decimal += str(remainder // down)
    #更新余数
    remainder %= down

print(decimal)
