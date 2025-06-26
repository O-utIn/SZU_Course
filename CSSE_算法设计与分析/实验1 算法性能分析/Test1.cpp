#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
using namespace std;

// 选择排序函数
void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        // 假设当前未排序部分的第一个元素是最小的
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        // 将最小元素与未排序部分的第一个元素交换位置
        swap(arr[i], arr[minIndex]);
    }
}

// 冒泡排序函数
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                // 如果当前元素大于下一个元素，则交换它们
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// 插入排序函数
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    // 从第二个元素开始，将其插入到前面已排序的部分
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        // 当找到一个比当前元素大的元素时，将该元素后移
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        // 将当前元素插入到正确的位置
        arr[j + 1] = key;
    }
}

// 合并排序的辅助函数，用于合并两个已排序的子数组
void merge(vector<int>& arr, int left, int mid, int right) {
    // 计算左右子数组的长度
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // 创建两个临时数组，分别存储左右子数组
    vector<int> L(n1), R(n2);

    // 将左子数组的元素复制到临时数组 L 中
    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    // 将右子数组的元素复制到临时数组 R 中
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    // 初始化三个指针，分别用于遍历 L、R 和原数组
    int i = 0, j = 0, k = left;

    // 比较 L 和 R 中的元素，将较小的元素依次放入原数组中
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    // 将 L 中剩余的元素复制到原数组中
    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    // 将 R 中剩余的元素复制到原数组中
    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

// 合并排序函数，采用分治法将数组递归地分成两部分，分别排序后再合并
void mergeSort(vector<int>& arr, int left, int right) {
    // 当左索引小于右索引时，继续递归排序
    if (left < right) {
        // 计算中间索引
        int mid = left + (right - left) / 2;

        // 递归排序左子数组
        mergeSort(arr, left, mid);
        // 递归排序右子数组
        mergeSort(arr, mid + 1, right);

        // 合并两个子数组
        merge(arr, left, mid, right);
    }
}

// 快速排序的辅助函数，用于选择一个基准元素，将数组分为两部分，左边部分小于基准，右边部分大于基准
int partition(vector<int>& arr, int low, int high) {
    // 选择最后一个元素作为基准元素
    int pivot = arr[high];
    // 初始化一个指针 i，用于标记小于基准元素的元素的位置
    int i = low - 1;

    // 遍历数组，将小于基准元素的元素交换到左边
    for (int j = low; j <= high - 1; ++j) {
        if (arr[j] < pivot) {
            ++i;
            swap(arr[i], arr[j]);
        }
    }
    // 将基准元素放到正确的位置
    swap(arr[i + 1], arr[high]);
    // 返回基准元素的最终位置
    return i + 1;
}

// 快速排序函数，采用分治法，通过基准元素将数组分成两部分，分别对两部分进行排序
void quickSort(vector<int>& arr, int low, int high) {
    // 当起始索引小于结束索引时，继续递归排序
    if (low < high) {
        // 找到基准元素的最终位置
        int pi = partition(arr, low, high);

        // 递归排序基准元素左边的部分
        quickSort(arr, low, pi - 1);
        // 递归排序基准元素右边的部分
        quickSort(arr, pi + 1, high);
    }
}

// 该函数用于测量指定排序函数对给定大小数组进行排序的平均运行时间
double measureTime(void (*sortFunction)(vector<int>&), int n) {
    // 创建一个随机数生成器，用于生成随机数
    random_device rd;
    mt19937 gen(rd());
    // 定义一个均匀分布，生成 1 到 1000000 之间的随机整数
    uniform_int_distribution<> dis(1, 1000000);


    double totalTime = 0.0;
    // 进行 20 次测试，获取平均时间
    for (int i = 0; i < 20; ++i) {
        // 创建一个大小为 n 的向量，用于存储随机生成的数组
        vector<int> arr(n);
        // 为数组的每个元素生成一个随机数
        for (int j = 0; j < n; ++j) {
            arr[j] = dis(gen);
        }

        // 记录排序开始的时间点
        auto start = chrono::high_resolution_clock::now();
        // 调用排序函数对数组进行排序
        sortFunction(arr);
        // 记录排序结束的时间点
        auto end = chrono::high_resolution_clock::now();

        // 计算排序所花费的时间
        chrono::duration<double> elapsed = end - start;
        totalTime += elapsed.count();
    }
    // 返回 20 次测试的平均运行时间
    return totalTime / 20;
}

int main() {
    vector<int> sizes = {10, 100, 1000, 10000, 100000, 200000, 300000, 400000, 500000};

    for (int size : sizes) {
        cout << "Array size: " << size << endl;

        // 输出选择排序的平均运行时间
        cout << "Selection Sort average time: " << measureTime(selectionSort, size) << " seconds" << endl;
        // 输出冒泡排序的平均运行时间
        cout << "Bubble Sort average time: " << measureTime(bubbleSort, size) << " seconds" << endl;
        // 输出插入排序的平均运行时间
        cout << "Insertion Sort average time: " << measureTime(insertionSort, size) << " seconds" << endl;

        // 定义一个 lambda 函数，用于调用合并排序函数
        auto mergeSortWrapper = [](vector<int>& arr) {
            mergeSort(arr, 0, arr.size() - 1);
        };
        // 输出合并排序的平均运行时间
        cout << "Merge Sort average time: " << measureTime(mergeSortWrapper, size) << " seconds" << endl;

        // 定义一个 lambda 函数，用于调用快速排序函数
        auto quickSortWrapper = [](vector<int>& arr) {
            quickSort(arr, 0, arr.size() - 1);
        };
        // 输出快速排序的平均运行时间
        cout << "Quick Sort average time: " << measureTime(quickSortWrapper, size) << " seconds" << endl;
        cout << endl;
    }

    return 0;
}
