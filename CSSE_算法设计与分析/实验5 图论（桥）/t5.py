import time

def find_bridges(n, edges):
    adj = [[] for _ in range(n)]
    for u, v in edges:
        adj[u].append(v)
        adj[v].append(u)
    
    def dfs(u, visited):
        visited[u] = True
        for v in adj[u]:
            if not visited[v]:
                dfs(v, visited)
    
    def count_components():
        visited = [False] * n
        count = 0
        for i in range(n):
            if not visited[i]:
                dfs(i, visited)
                count += 1
        return count
    
    original = count_components()
    bridges = []
    
    for u, v in edges:
        adj[u].remove(v)
        adj[v].remove(u)
        new = count_components()
        adj[u].append(v)
        adj[v].append(u)
        if new == original + 1:
            bridges.append((u, v))
    
    return bridges

class UnionFind:
    def __init__(self, size):
        self.father = list(range(size))
        self.rank = [1] * size
    
    def find(self, u):
        # 迭代路径压缩（提升find速度，不影响秩）
        path = []
        while self.father[u] != u:
            path.append(u)
            u = self.father[u]
        for v in path:
            self.father[v] = u
        return u
    
    def union(self, u, v):
        ru = self.find(u)
        rv = self.find(v)
        if ru == rv:
            return False
        if self.rank[ru] > self.rank[rv]:
            ru, rv = rv, ru
        self.father[ru] = rv
        if self.rank[ru] == self.rank[rv]:
            self.rank[rv] += 1
        return True
    
    def reset(self):
        # 重置并查集，避免重复初始化
        for i in range(len(self.father)):
            self.father[i] = i
            self.rank[i] = 1

def find_bridges(n, edges):
    uf = UnionFind(n)
    bridges = []
    m = len(edges)
    for i in range(m):
        uf.reset()
        for j in range(m):
            if j != i:
                uf.union(edges[j][0], edges[j][1])
        u, v = edges[i]
        if uf.find(u) != uf.find(v):
            bridges.append((u, v))
    return bridges

def read_graph_from_file(filename):
    with open(filename, 'r') as f:
        lines = f.read().splitlines()
    
    n = int(lines[0].strip())
    m = int(lines[1].strip())
    
    edges = []
    for line in lines[2:2+m]:
        u, v = map(int, line.strip().split())
        edges.append((u, v))
    
    unique_edges = list(set(tuple(sorted(e)) for e in edges))
    print(f"读取了 {m} 条边，去重后 {len(unique_edges)} 条")
    
    return n, unique_edges

# 图2的边列表（16顶点，6条桥）
n = 16
edges = [
    (0, 1),
    (2, 3),
    (2, 6), (6, 7),
    (9, 10),
    (12, 13),
    (4, 8), (4, 9), (8, 9), (8, 13), (9, 13), (10, 11), (10, 14), (11, 15), (14, 15)
]


if __name__ == "__main__":
    start_time = time.time() * 1000
    bridges = find_bridges(n, edges)
    end_time = time.time() * 1000
    
    for u, v in bridges:
        print(f"边({u},{v}) 是桥")

    print("桥的数量=", len(bridges))
    print(f"运行时间：{end_time - start_time:.0f} 毫秒")