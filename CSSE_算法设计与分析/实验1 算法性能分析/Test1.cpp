#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
using namespace std;

// ѡ��������
void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        // ���赱ǰδ���򲿷ֵĵ�һ��Ԫ������С��
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        // ����СԪ����δ���򲿷ֵĵ�һ��Ԫ�ؽ���λ��
        swap(arr[i], arr[minIndex]);
    }
}

// ð��������
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                // �����ǰԪ�ش�����һ��Ԫ�أ��򽻻�����
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ����������
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    // �ӵڶ���Ԫ�ؿ�ʼ��������뵽ǰ��������Ĳ���
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        // ���ҵ�һ���ȵ�ǰԪ�ش��Ԫ��ʱ������Ԫ�غ���
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        // ����ǰԪ�ز��뵽��ȷ��λ��
        arr[j + 1] = key;
    }
}

// �ϲ�����ĸ������������ںϲ������������������
void merge(vector<int>& arr, int left, int mid, int right) {
    // ��������������ĳ���
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // ����������ʱ���飬�ֱ�洢����������
    vector<int> L(n1), R(n2);

    // �����������Ԫ�ظ��Ƶ���ʱ���� L ��
    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    // �����������Ԫ�ظ��Ƶ���ʱ���� R ��
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    // ��ʼ������ָ�룬�ֱ����ڱ��� L��R ��ԭ����
    int i = 0, j = 0, k = left;

    // �Ƚ� L �� R �е�Ԫ�أ�����С��Ԫ�����η���ԭ������
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

    // �� L ��ʣ���Ԫ�ظ��Ƶ�ԭ������
    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    // �� R ��ʣ���Ԫ�ظ��Ƶ�ԭ������
    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

// �ϲ������������÷��η�������ݹ�طֳ������֣��ֱ�������ٺϲ�
void mergeSort(vector<int>& arr, int left, int right) {
    // ��������С��������ʱ�������ݹ�����
    if (left < right) {
        // �����м�����
        int mid = left + (right - left) / 2;

        // �ݹ�������������
        mergeSort(arr, left, mid);
        // �ݹ�������������
        mergeSort(arr, mid + 1, right);

        // �ϲ�����������
        merge(arr, left, mid, right);
    }
}

// ��������ĸ�������������ѡ��һ����׼Ԫ�أ��������Ϊ�����֣���߲���С�ڻ�׼���ұ߲��ִ��ڻ�׼
int partition(vector<int>& arr, int low, int high) {
    // ѡ�����һ��Ԫ����Ϊ��׼Ԫ��
    int pivot = arr[high];
    // ��ʼ��һ��ָ�� i�����ڱ��С�ڻ�׼Ԫ�ص�Ԫ�ص�λ��
    int i = low - 1;

    // �������飬��С�ڻ�׼Ԫ�ص�Ԫ�ؽ��������
    for (int j = low; j <= high - 1; ++j) {
        if (arr[j] < pivot) {
            ++i;
            swap(arr[i], arr[j]);
        }
    }
    // ����׼Ԫ�طŵ���ȷ��λ��
    swap(arr[i + 1], arr[high]);
    // ���ػ�׼Ԫ�ص�����λ��
    return i + 1;
}

// ���������������÷��η���ͨ����׼Ԫ�ؽ�����ֳ������֣��ֱ�������ֽ�������
void quickSort(vector<int>& arr, int low, int high) {
    // ����ʼ����С�ڽ�������ʱ�������ݹ�����
    if (low < high) {
        // �ҵ���׼Ԫ�ص�����λ��
        int pi = partition(arr, low, high);

        // �ݹ������׼Ԫ����ߵĲ���
        quickSort(arr, low, pi - 1);
        // �ݹ������׼Ԫ���ұߵĲ���
        quickSort(arr, pi + 1, high);
    }
}

// �ú������ڲ���ָ���������Ը�����С������������ƽ������ʱ��
double measureTime(void (*sortFunction)(vector<int>&), int n) {
    // ����һ����������������������������
    random_device rd;
    mt19937 gen(rd());
    // ����һ�����ȷֲ������� 1 �� 1000000 ֮����������
    uniform_int_distribution<> dis(1, 1000000);


    double totalTime = 0.0;
    // ���� 20 �β��ԣ���ȡƽ��ʱ��
    for (int i = 0; i < 20; ++i) {
        // ����һ����СΪ n �����������ڴ洢������ɵ�����
        vector<int> arr(n);
        // Ϊ�����ÿ��Ԫ������һ�������
        for (int j = 0; j < n; ++j) {
            arr[j] = dis(gen);
        }

        // ��¼����ʼ��ʱ���
        auto start = chrono::high_resolution_clock::now();
        // �����������������������
        sortFunction(arr);
        // ��¼���������ʱ���
        auto end = chrono::high_resolution_clock::now();

        // �������������ѵ�ʱ��
        chrono::duration<double> elapsed = end - start;
        totalTime += elapsed.count();
    }
    // ���� 20 �β��Ե�ƽ������ʱ��
    return totalTime / 20;
}

int main() {
    vector<int> sizes = {10, 100, 1000, 10000, 100000, 200000, 300000, 400000, 500000};

    for (int size : sizes) {
        cout << "Array size: " << size << endl;

        // ���ѡ�������ƽ������ʱ��
        cout << "Selection Sort average time: " << measureTime(selectionSort, size) << " seconds" << endl;
        // ���ð�������ƽ������ʱ��
        cout << "Bubble Sort average time: " << measureTime(bubbleSort, size) << " seconds" << endl;
        // ������������ƽ������ʱ��
        cout << "Insertion Sort average time: " << measureTime(insertionSort, size) << " seconds" << endl;

        // ����һ�� lambda ���������ڵ��úϲ�������
        auto mergeSortWrapper = [](vector<int>& arr) {
            mergeSort(arr, 0, arr.size() - 1);
        };
        // ����ϲ������ƽ������ʱ��
        cout << "Merge Sort average time: " << measureTime(mergeSortWrapper, size) << " seconds" << endl;

        // ����һ�� lambda ���������ڵ��ÿ���������
        auto quickSortWrapper = [](vector<int>& arr) {
            quickSort(arr, 0, arr.size() - 1);
        };
        // ������������ƽ������ʱ��
        cout << "Quick Sort average time: " << measureTime(quickSortWrapper, size) << " seconds" << endl;
        cout << endl;
    }

    return 0;
}
