#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <string>
#include <algorithm>

// 检查颜色 c 对顶点 v 是否安全
bool isSafe(int v, const std::vector<std::vector<int>>& graph, const std::vector<int>& color, int c) {
    for (int i : graph[v]) {
        if (color[i] == c) {
            return false;
        }
    }
    return true;
}

// 计算顶点 v 的可选颜色数
int getAvailableColors(int v, const std::vector<std::vector<int>>& graph, const std::vector<int>& color, int m) {
    int count = 0;
    for (int c = 1; c <= m; ++c) {
        if (isSafe(v, graph, color, c)) {
            ++count;
        }
    }
    return count;
}

// 选择下一个要染色的顶点，优先选择可选颜色数少的顶点
int selectNextVertex(const std::vector<std::vector<int>>& graph, const std::vector<int>& color, int m) {
    int minAvailableColors = m + 1;
    int nextVertex = -1;
    for (size_t v = 0; v < graph.size(); ++v) {
        if (color[v] == 0) {
            int availableColors = getAvailableColors(v, graph, color, m);
            if (availableColors < minAvailableColors) {
                minAvailableColors = availableColors;
                nextVertex = v;
            }
        }
    }
    return nextVertex;
}

// 可行性剪枝：检查染色后相邻节点是否还有可选颜色
bool isPruningValid(const std::vector<std::vector<int>>& graph, const std::vector<int>& color, int v, int m) {
    for (int adj : graph[v]) {
        if (color[adj] == 0 && getAvailableColors(adj, graph, color, m) == 0) {
            return false;
        }
    }
    return true;
}

// 计算节点数为 3 时的染色方案数
int calculateAns3(int m, bool isCycle) {
    if (isCycle) {
        return m * (m - 1) * (m - 2);
    } else {
        int same = m * (m - 1);
        int diff = m * (m - 1) * (m - 2);
        return same + diff;
    }
}

// 回溯法求解图着色
void dfsPro(const std::vector<std::vector<int>>& graph, std::vector<int>& color, int m, int numRegions, int& solutionCount, bool isFirstThreeFixed) {
    int nextVertex = selectNextVertex(graph, color, m);
    if (nextVertex == -1) {
        // 所有顶点都已染色，找到一个可行方案
        ++solutionCount;
        return;
    }

    for (int c = 1; c <= m; ++c) {
        if (isSafe(nextVertex, graph, color, c)) {
            color[nextVertex] = c;
            // 可行性剪枝
            if (isPruningValid(graph, color, nextVertex, m)) {
                dfsPro(graph, color, m, numRegions, solutionCount, isFirstThreeFixed);
            }
            color[nextVertex] = 0; // 回溯
        }
    }
}

// 初始化并调用求解
int graphColoring(const std::vector<std::vector<int>>& graph, int m, bool isCycle) {
    int numRegions = graph.size();
    if (numRegions == 1) {
        return m;
    } else if (numRegions == 2) {
        return m * (m - 1);
    } else if (numRegions == 3) {
        return calculateAns3(m, isCycle);
    }

    // 固定前三个节点
    std::vector<int> color(numRegions, 0);
    std::vector<int> fixedColors = {1, 2, 3};
    int solutionCountWithFixed = 0;
    do {
        color[0] = fixedColors[0];
        color[1] = fixedColors[1];
        color[2] = fixedColors[2];
        if (isSafe(0, graph, color, fixedColors[0]) && isSafe(1, graph, color, fixedColors[1]) && isSafe(2, graph, color, fixedColors[2])) {
            int solutionCount = 0;
            dfsPro(graph, color, m, numRegions, solutionCount, true);
            solutionCountWithFixed += solutionCount;
        }
    } while (std::next_permutation(fixedColors.begin(), fixedColors.end()));

    int ans3 = calculateAns3(m, isCycle);
    return solutionCountWithFixed * ans3;
}

int main() {
    std::string line;
    std::ifstream file("le450_5a.col"); // 请确保文件路径正确
    std::vector<std::vector<int>> graph;
    int vertices = 0, edges = 0;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (line.substr(0, 2) == "c ") continue; // 跳过注释行
            if (line.substr(0, 2) == "p ") {
                int v, e;
                std::sscanf(line.c_str(), "p edge %d %d", &v, &e);
                graph.resize(v); // 顶点从 0 开始编号
                vertices = v;
                edges = e;
            } else if (line.substr(0, 2) == "e ") {
                int u, w;
                std::sscanf(line.c_str(), "e %d %d", &u, &w);
                // 顶点编号减 1 以从 0 开始
                --u; --w;
                graph[u].push_back(w);
                graph[w].push_back(u); // 无向图，双向添加边
            }
        }
        file.close();
    } else {
        std::cerr << "无法打开文件" << std::endl;
        return 1;
    }

    int m = 5; // 5色
    bool isCycle = false; // 需要根据实际情况判断前三个节点是否构成环
    auto start = std::chrono::high_resolution_clock::now();
    int solutionCount = graphColoring(graph, m, isCycle);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "节点数: " << vertices << std::endl;
    std::cout << "边数: " << edges << std::endl;
    std::cout << "可行方案数: " << solutionCount << std::endl;
    std::cout << "得到所有方案所需时间: " << duration << " 毫秒" << std::endl;

    return 0;
}    