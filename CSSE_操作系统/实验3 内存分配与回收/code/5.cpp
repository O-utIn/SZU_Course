#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fstream>
#include <algorithm>
#include <random>
#include <sys/mman.h> // 新增：内存映射头文件

using namespace std;
using namespace std::chrono;

constexpr size_t PAGE_SIZE = 4096; // 4KB页大小
constexpr size_t PAGES_PER_GROUP = 16; // 每组16页
constexpr size_t GROUP_SIZE = PAGES_PER_GROUP * PAGE_SIZE; // 组大小

// 内存状态结构体
struct MemoryStatus {
    size_t vmSize;    // 虚拟内存大小 (KB)
    size_t vmRSS;     // 物理内存驻留集大小 (KB)
    size_t vmHWM;     // 物理内存峰值 (KB)
    size_t pageFaults; // 页面错误次数
};

// 从/proc/pid/status读取内存状态
MemoryStatus getMemoryStatus(pid_t pid) {
    MemoryStatus status = {0, 0, 0, 0};
    string path = "/proc/" + to_string(pid) + "/status";
    ifstream file(path);
    
    if (!file.is_open()) {
        cerr << "无法打开 " << path << endl;
        return status;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.find("VmSize:") != string::npos) {
            sscanf(line.c_str(), "VmSize:\t%zu kB", &status.vmSize);
        } else if (line.find("VmRSS:") != string::npos) {
            sscanf(line.c_str(), "VmRSS:\t%zu kB", &status.vmRSS);
        } else if (line.find("VmHWM:") != string::npos) {
            sscanf(line.c_str(), "VmHWM:\t%zu kB", &status.vmHWM);
        } else if (line.find("majflt:") != string::npos) {
            // 读取主要页面错误次数
            sscanf(line.c_str(), "majflt:\t%zu", &status.pageFaults);
        }
    }
    
    return status;
}

// 打印内存状态
void printMemoryStatus(const MemoryStatus& status, const string& message) {
    cout << "\n" << message << "：" << endl;
    cout << "  VmSize: " << status.vmSize / 1024.0 << " MB" << endl;
    cout << "  VmRSS:  " << status.vmRSS / 1024.0 << " MB" << endl;
    cout << "  VmHWM:  " << status.vmHWM / 1024.0 << " MB" << endl;
    cout << "  页面错误次数: " << status.pageFaults << endl;
}

// 测量函数执行时间
template<typename Func>
long long measureTime(Func func) {
    auto start = high_resolution_clock::now();
    func();
    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start).count();
}

// 模式1：随机访问（破坏空间局部性）
void accessRandomly(char* memory, size_t totalSize, int iterations) {
    // 生成随机访问序列
    vector<size_t> offsets;
    for (size_t i = 0; i < totalSize; i += PAGE_SIZE) {
        offsets.push_back(i);
    }
    
    // 使用Mersenne Twister算法打乱顺序
    mt19937 rng(random_device{}());
    shuffle(offsets.begin(), offsets.end(), rng);
    
    for (int iter = 0; iter < iterations; ++iter) {
        for (size_t offset : offsets) {
            memory[offset]++; // 写操作
        }
    }
}

// 模式2：优化的16页连续组访问（增强空间局部性）
void accessBy16PageGroups(char* memory, size_t totalSize, int iterations) {
    // 预取每个组的第一个页面，提高缓存命中率
    for (int iter = 0; iter < iterations; ++iter) {
        for (size_t groupStart = 0; groupStart < totalSize; groupStart += GROUP_SIZE) {
            // 预取当前组
            __builtin_prefetch(&memory[groupStart], 1, 3);
            
            size_t groupEnd = min(groupStart + GROUP_SIZE, totalSize);
            for (size_t i = groupStart; i < groupEnd; i += 64) { // 按缓存行大小访问
                memory[i]++;
            }
        }
    }
}

// 获取系统内存信息
size_t getTotalMemory() {
    ifstream meminfo("/proc/meminfo");
    string line;
    size_t totalMem = 0;
    
    while (getline(meminfo, line)) {
        if (line.find("MemTotal:") != string::npos) {
            sscanf(line.c_str(), "MemTotal: %zu kB", &totalMem);
            break;
        }
    }
    
    return totalMem * 1024; // 转换为字节
}

int main() {
    pid_t pid = getpid();
    cout << "进程ID: " << pid << endl;
    
    // 获取系统总内存
    size_t totalMemory = getTotalMemory();
    cout << "系统总内存: " << totalMemory / (1024 * 1024) << " MB" << endl;
    
    // 分配2倍系统内存，强制触发页面置换
    size_t allocationSize = totalMemory * 1;
    cout << "计划分配内存: " << allocationSize / (1024 * 1024) << " MB" << endl;
    
    // 分配内存（使用mmap以便于控制）
    char* memory = (char*)mmap(nullptr, allocationSize, PROT_READ | PROT_WRITE, 
                              MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (memory == MAP_FAILED) {
        cerr << "内存分配失败" << endl;
        return 1;
    }
    
    // 初始化内存，强制物理页分配
    cout << "正在初始化内存..." << endl;
    for (size_t i = 0; i < allocationSize; i += 4096) {
        memory[i] = 0;
        if (i % (1024 * 1024 * 10) == 0) { // 每10MB更新一次进度
            cout << "\r初始化进度: " << (i * 100 / allocationSize) << "%";
            cout.flush();
        }
    }
    cout << "\r初始化进度: 100%" << endl;
    
    // 初始内存状态
    MemoryStatus initialStatus = getMemoryStatus(pid);
    printMemoryStatus(initialStatus, "初始内存状态");
    
    // 设置访问参数
    int iterations = 3; // 减少迭代次数，避免测试时间过长
    size_t totalAccesses = (allocationSize / PAGE_SIZE) * iterations;
    
    cout << "\n测试参数:" << endl;
    cout << "  总访问遍数: " << iterations << endl;
    cout << "  总访问次数: " << totalAccesses << endl;
    cout << "  模式1: 真正随机访问（破坏空间局部性）" << endl;
    cout << "  模式2: 16页连续组访问（增强空间局部性）" << endl;
    
    // 模式1：随机访问
    cout << "\n正在执行模式1测试..." << endl;
    MemoryStatus beforePattern1 = getMemoryStatus(pid);
    long long time1 = measureTime([&]() {
        accessRandomly(memory, allocationSize, iterations);
    });
    MemoryStatus afterPattern1 = getMemoryStatus(pid);
    
    cout << "模式1执行时间: " << time1 << " 毫秒" << endl;
    cout << "模式1页面错误增加: " << (afterPattern1.pageFaults - beforePattern1.pageFaults) << endl;
    
    // 重置内存内容
    cout << "\n重置内存内容..." << endl;
    for (size_t i = 0; i < allocationSize; i += 4096) {
        memory[i] = 0;
    }
    
    // 模式2：连续组访问
    cout << "\n正在执行模式2测试..." << endl;
    MemoryStatus beforePattern2 = getMemoryStatus(pid);
    long long time2 = measureTime([&]() {
        accessBy16PageGroups(memory, allocationSize, iterations);
    });
    MemoryStatus afterPattern2 = getMemoryStatus(pid);
    
    cout << "模式2执行时间: " << time2 << " 毫秒" << endl;
    cout << "模式2页面错误增加: " << (afterPattern2.pageFaults - beforePattern2.pageFaults) << endl;
    
    // 计算性能差异
    double speedup = (double)time1 / time2;
    cout << "\n性能对比:" << endl;
    cout << fixed << setprecision(2);
    cout << "  模式2比模式1快: " << speedup << " 倍" << endl;
    
    // 最终内存状态
    printMemoryStatus(afterPattern2, "测试结束后的内存状态");
    
    // 释放内存
    munmap(memory, allocationSize);
    
    return 0;
}    
