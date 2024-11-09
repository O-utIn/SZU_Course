def find(grid, x, y, k, home_x, home_y, visited):
    if k == 0 or x < 0 or x >= len(grid) or y < 0 or y >= len(grid[0]):
        return 0

    nuts = 0

    if grid[x][y] > 0 and not visited[x][y]:
        nuts += grid[x][y]
        grid[x][y] = 0
        visited[x][y] = True
        # 在每次递归调用之前将当前位置标记为已访问

        # 将小松鼠的位置重置回家
        nuts += find(grid, home_x, home_y, k - 1, home_x, home_y, visited)

        nuts += find(grid, x - 1, y, k - 1, home_x, home_y, visited)  # 向上移动
        nuts += find(grid, x + 1, y, k - 1, home_x, home_y, visited)  # 向下移动
        nuts += find(grid, x, y - 1, k - 1, home_x, home_y, visited)  # 向左移动
        nuts += find(grid, x, y + 1, k - 1, home_x, home_y, visited)  # 向右移动

        # 在递归调用结束后将当前位置标记为未访问，使其他路径可以经过此位置
        visited[x][y] = False

        # 在每次递归调用结束后将小松鼠的位置重置回当前位置
        grid[x][y] = 1

    else:
        nuts += find(grid, x - 1, y, k - 1, home_x, home_y, visited)  # 向上移动
        nuts += find(grid, x + 1, y, k - 1, home_x, home_y, visited)  # 向下移动
        nuts += find(grid, x, y - 1, k - 1, home_x, home_y, visited)  # 向左移动
        nuts += find(grid, x, y + 1, k - 1, home_x, home_y, visited)  # 向右移动

    return nuts

mn_str = input("请输入M和N的值：")
m, n = map(int, mn_str.split(','))

xy_str = input("请输入小松鼠家的位置：")
x, y = map(int, xy_str.split(','))

k = int(input("请输入最多可以移动的小格子数："))

grid = []
visited = [[False] * n for j in range(m)]  # 用于标记位置是否已经访问过

#调用函数进行计算
for i in range(m):
    row_str = input("请输入第{}行小格子的值：".format(i+1))
    row = list(map(int, row_str.split(',')))
    grid.append(row)

nuts = find(grid, x - 1, y - 1, k, x - 1, y - 1, visited)  # 将家的位置作为参数传递给find函数
print("小松鼠最多可以储藏的坚果数量为：", nuts)
