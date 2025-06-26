#include <iostream>
#include <vector>
#include <chrono>

// �����ɫ c �Զ��� v �Ƿ�ȫ
bool isSafe(int v, const std::vector<std::vector<int>>& graph, const std::vector<int>& color, int c) {
    for (int i : graph[v]) {
        if (color[i] == c) {
            return false;
        }
    }
    return true;
}

// ���ݷ����ͼ��ɫ
void graphColoringUtil(const std::vector<std::vector<int>>& graph, std::vector<int>& color, int m, int v, int numRegions, int& solutionCount) {
    if (v == numRegions) {
        // �ҵ�һ�����з������������� 1
        solutionCount++;
        return;
    }

    for (int c = 1; c <= m; c++) {
        if (isSafe(v, graph, color, c)) {
            color[v] = c;
            graphColoringUtil(graph, color, m, v + 1, numRegions, solutionCount);
            color[v] = 0; // ����
        }
    }
}

// ��ʼ�����������
int graphColoring(const std::vector<std::vector<int>>& graph, int m) {
    int numRegions = graph.size();
    std::vector<int> color(numRegions, 0);
    int solutionCount = 0;
    graphColoringUtil(graph, color, m, 0, numRegions, solutionCount);
    return solutionCount;
}

int main() {
    // ����ͼ���� 9 ��������ڽӱ����������ʵ��ͼ�������ӹ�ϵ�޸ģ�
    std::vector<std::vector<int>> graph = {
        {2, 3, 4},        // ���� 1 �������� 2��3��4
        {1, 3, 4, 5},     // ���� 2 �������� 1��3��4��5
        {1, 2, 4}, 
        {1, 2, 3, 5, 6, 7}, 
        {2, 4, 6, 8}, 
        {4, 5, 7, 8, 9},  
        {4, 6, 9},
        {5, 6, 9},  
        {6, 7, 8}      
    };
    int m = 4; // ��ɫ

    // ����ڵ���
    int nodeCount = graph.size();

    // �������
    int edgeCount = 0;
    for (const auto& neighbors : graph) {
        edgeCount += neighbors.size();
    }
    edgeCount /= 2; // ÿ���߱������ڵ����¼һ��

    auto start = std::chrono::high_resolution_clock::now();
    int solutionCount = graphColoring(graph, m);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "�ڵ���: " << nodeCount << std::endl;
    std::cout << "����: " << edgeCount << std::endl;
    std::cout << "���з�����: " << solutionCount << std::endl;
    std::cout << "�õ����з�������ʱ��: " << duration << " ����" << std::endl;

    return 0;
}    
