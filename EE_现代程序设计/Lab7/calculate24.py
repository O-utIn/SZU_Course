def calculate(nums):
    #初始化计数器
    count = 0
    
    #在加法/减法/乘法中遍历
    for cal1 in ['+','-','*']:
        for cal2 in ['+','-','*']:
            for cal3 in ['+','-','*']:
                #构建算式
                expression = f"{nums[0]}{cal1}{nums[1]}{cal2}{nums[2]}{cal3}{nums[3]}"
                #计算算式的结果
                result = eval(expression)
                
                #将结果与24比对，若结果为24，计数器加一
                if result == 24:
                    count += 1
    return count

#获得用户输入并将数据保存在列表中
nums = list(map(int,input('请输入四个1到10之间的各不相同的数字，以空格分隔:').split()))

#调用计算函数得出结果
result = calculate(nums)
print(result)
