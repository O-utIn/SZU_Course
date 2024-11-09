# 获取群岛地形的长度和宽度
M,N = map(int,input("请输入群岛地形的长度和宽度，以空格分隔:").split())
# 创建一个空的群岛地形列表
grid = []

# 用户输入的每行岛屿分布情况存储在grid列表中
for k in range(M):
    row = list(map(int,input("请输入第{}行岛屿的分布:".format(k+1)).split()))
    grid.append(row)
    
# 创建一个与群岛地形大小相同的布尔值二维列表，用于跟踪已经访问过的岛屿
visited = [[False] * N for _ in range(M)]

# 初始化最大面积和最小面积
max_area = 0
min_area = float('inf')

# 定义函数用于计算岛屿面积
def find(i,j):
    # 检查当前位置是否越界或者已经访问过
    if i < 0 or i >=M or j < 0 or j >= N:
        return 0
    if visited[i][j] or grid[i][j] == 0:
        return 0
    
    # 标记当前位置为已访问
    visited[i][j] = True
    # 初始化当前岛屿面积为1
    area = 1

    # 调用函数计算与当前位置相连的岛屿面积
    area += find(i - 1,j) # 上
    area += find(i + 1,j) # 下
    area += find(i,j + 1) # 左
    area += find(i,j - 1) # 右
    # 返回当前岛屿面积
    return area

# 遍历群岛地形的每个位置
for i in range(M):
    for j in range(N):
        # 如果当前位置是一个未访问过的岛屿
        if grid[i][j] == 1 and not visited[i][j]:
            # 调用函数计算当前岛屿的面积
            area = find(i,j)

            # 更新最大面积和最小面积
            max_area = max(max_area,area)
            min_area = min(min_area,area)

# 计算最大面积和最小面积之差并打印结果
area_diff = max_area - min_area
print(area_diff)

