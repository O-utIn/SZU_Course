N = int(input("请输入纸片完整边的长度:"))
heights = list(map(int,input("请输入每列格子残存部分的高度，用空格隔开:").split()))

max_area = 0
stack = [] #用于记录高度的索引


for i, height in enumerate(heights):
    while stack and height < heights[stack[-1]]:
        #当前高度小于栈索引对应的高度，计算栈项高度为高的矩形面积
        top = stack.pop() #弹出索引

        #计算矩形的宽度
        width = i if not stack else i - stack[-1] - 1
        #计算矩形的面积
        area = heights[top] * width
        #更新最大面积
        max_area = max(max_area,area)
    stack.append(i) #将当前索引放入栈

while stack:
    #遍历完所有列后，如果还有剩余项，说明这些列还能继续计算
    top = stack.pop() #弹出栈索引

    #计算矩形的宽度
    width = N if not stack else N - stack[-1] - 1
    #计算矩形的面积
    area = heights[top] * width
    #更新最大面积
    max_area = max(max_area,area)

#输出结果
print("能够剪出的最大矩形面积为:",max_area)
