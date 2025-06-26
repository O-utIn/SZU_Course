#include <iostream>
#include <vector>
#include <chrono>

// 检查颜色 c 对顶点 v 是否安全
bool isSafe(int v, const std::vector<std::vector<int>>& graph, const std::vector<int>& color, int c) {
    for (int i : graph[v]) {
        if (color[i] == c) {
            return false;
        }
    }
    return true;
}

// 回溯法求解图着色
void graphColoringUtil(const std::vector<std::vector<int>>& graph, std::vector<int>& color, int m, int v, int numRegions, int& solutionCount) {
    if (v == numRegions) {
        // 找到一个可行方案，方案数加 1
        solutionCount++;
        return;
    }

    for (int c = 1; c <= m; c++) {
        if (isSafe(v, graph, color, c)) {
            color[v] = c;
            graphColoringUtil(graph, color, m, v + 1, numRegions, solutionCount);
            color[v] = 0; // 回溯
        }
    }
}

// 初始化并调用求解
int graphColoring(const std::vector<std::vector<int>>& graph, int m) {
    int numRegions = graph.size();
    std::vector<int> color(numRegions, 0);
    int solutionCount = 0;
    graphColoringUtil(graph, color, m, 0, numRegions, solutionCount);
    return solutionCount;
}

int main() {
    // 假设图二有 9 个区域的邻接表（这里需根据实际图二的连接关系修改）
    std::vector<std::vector<int>> graph = {
        {2, 3, 4},        // 区域 1 相邻区域 2、3、4
        {1, 3, 4, 5},     // 区域 2 相邻区域 1、3、4、5
        {1, 2, 4}, 
        {1, 2, 3, 5, 6, 7}, 
        {2, 4, 6, 8}, 
        {4, 5, 7, 8, 9},  
        {4, 6, 9},
        {5, 6, 9},  
        {6, 7, 8}      
    };
    int m = 4; // 四色

    // 计算节点数
    int nodeCount = graph.size();

    // 计算边数
    int edgeCount = 0;
    for (const auto& neighbors : graph) {
        edgeCount += neighbors.size();
    }
    edgeCount /= 2; // 每条边被两个节点各记录一次

    auto start = std::chrono::high_resolution_clock::now();
    int solutionCount = graphColoring(graph, m);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "节点数: " << nodeCount << std::endl;
    std::cout << "边数: " << edgeCount << std::endl;
    std::cout << "可行方案数: " << solutionCount << std::endl;
    std::cout << "得到所有方案所需时间: " << duration << " 毫秒" << std::endl;

    return 0;
}    
