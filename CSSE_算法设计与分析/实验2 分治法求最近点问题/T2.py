import random
import time
import math


# 生成随机点
def generate_points(N):
    return [(random.random(), random.random()) for _ in range(N)]


# 计算两点之间的距离
def distance(p1, p2):
    return math.sqrt((p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2)


# 蛮力法计算最短距离
def brute_force(points):
    n = len(points)
    min_dist = float('inf')
    for i in range(n):
        for j in range(i + 1, n):
            dist = distance(points[i], points[j])
            if dist < min_dist:
                min_dist = dist
    return min_dist


# 分治法计算最短距离
def closest_pair(points):
    def closest_pair_recursive(X, Y):
        n = len(X)
        if n <= 3:
            return brute_force(X)

        mid = n // 2
        mid_point = X[mid]

        XL = X[:mid]
        XR = X[mid:]

        YL = [p for p in Y if p[0] <= mid_point[0]]
        YR = [p for p in Y if p[0] > mid_point[0]]

        dl = closest_pair_recursive(XL, YL)
        dr = closest_pair_recursive(XR, YR)
        d = min(dl, dr)

        strip = [p for p in Y if abs(p[0] - mid_point[0]) < d]

        for i in range(len(strip)):
            for j in range(i + 1, min(i + 7, len(strip))):
                dist = distance(strip[i], strip[j])
                if dist < d:
                    d = dist

        return d

    X = sorted(points, key=lambda p: p[0])
    Y = sorted(points, key=lambda p: p[1])
    return closest_pair_recursive(X, Y)


# 测试不同 N 值下的算法运行时间
N_values = [100000, 200000, 300000, 400000, 500000, 1000000]

for N in N_values:
    points = generate_points(N)

    # 蛮力法计时
    start_time = time.time()
    brute_force(points)
    end_time = time.time()
    brute_time = end_time - start_time

    # 分治法计时
    start_time = time.time()
    closest_pair(points)
    end_time = time.time()
    divide_time = end_time - start_time

    print(f"点的规模 N = {N}")
    print(f"蛮力法实际运算时间: {brute_time:.6f} 秒")
    print(f"分治法实际运算时间: {divide_time:.6f} 秒")
    print("-" * 30)
