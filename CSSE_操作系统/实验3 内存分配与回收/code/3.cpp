#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/resource.h>
#include <iomanip>
#include <cstring>

using namespace std;

// 内存状态结构体
struct MemoryStatus {
    size_t vmSize;    // 虚拟内存大小 (KB)
    size_t vmRSS;     // 物理内存驻留集大小 (KB)
    size_t vmData;    // 数据段大小 (KB)
    size_t vmHWM;     // 物理内存峰值 (KB)
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
        } else if (line.find("VmData:") != string::npos) {
            sscanf(line.c_str(), "VmData:\t%zu kB", &status.vmData);
        } else if (line.find("VmHWM:") != string::npos) {
            sscanf(line.c_str(), "VmHWM:\t%zu kB", &status.vmHWM);
        }
    }
    
    return status;
}

// 打印内存状态
void printMemoryStatus(const MemoryStatus& status, const string& message) {
    cout << "\n" << message << "：" << endl;
    cout << "  VmSize: " << status.vmSize / 1024.0 << " MB" << endl;
    cout << "  VmRSS:  " << status.vmRSS / 1024.0 << " MB" << endl;
    cout << "  VmData: " << status.vmData / 1024.0 << " MB" << endl;
    cout << "  VmHWM:  " << status.vmHWM / 1024.0 << " MB" << endl;
}

// 等待用户输入
void waitForInput() {
    cout << "\n按Enter继续...";
    cin.ignore();
}

int main() {
    pid_t pid = getpid();
    cout << "进程ID: " << pid << endl;
    
    // 初始内存状态
    MemoryStatus initialStatus = getMemoryStatus(pid);
    printMemoryStatus(initialStatus, "初始内存状态");
    waitForInput();
    
    // 分配256MB内存
    const size_t size = 256 * 1024 * 1024; // 256MB
    char* memory = new char[size];
    memset(memory, 0, size); // 初始化内存，确保物理页被分配
    
    // 分配后的内存状态
    MemoryStatus allocatedStatus = getMemoryStatus(pid);
    printMemoryStatus(allocatedStatus, "分配256MB后的内存状态");
    waitForInput();
    
    // 每隔4KB读取一次内存
    const size_t interval = 4 * 1024; // 4KB
    for (size_t i = 0; i < size; i += interval) {
        char value = memory[i]; // 读取操作
    }
    
    // 读取后的内存状态
    MemoryStatus readStatus = getMemoryStatus(pid);
    printMemoryStatus(readStatus, "每隔4KB读取后的内存状态");
    waitForInput();
    
    // 每隔4KB写入一次内存
    for (size_t i = 0; i < size; i += interval) {
        memory[i] = 'A'; // 写入操作
    }
    
    // 写入后的内存状态
    MemoryStatus writeStatus = getMemoryStatus(pid);
    printMemoryStatus(writeStatus, "每隔4KB写入后的内存状态");
    waitForInput();
    
    // 释放内存
    delete[] memory;
    
    // 释放后的内存状态
    MemoryStatus finalStatus = getMemoryStatus(pid);
    printMemoryStatus(finalStatus, "释放内存后的内存状态");
    
    // 计算变化量
    cout << "\n=== 内存变化总结 ===" << endl;
    cout << "分配后 VmRSS 变化: " << (allocatedStatus.vmRSS - initialStatus.vmRSS) / 1024.0 << " MB" << endl;
    cout << "读取后 VmRSS 变化: " << (readStatus.vmRSS - allocatedStatus.vmRSS) / 1024.0 << " MB" << endl;
    cout << "写入后 VmRSS 变化: " << (writeStatus.vmRSS - readStatus.vmRSS) / 1024.0 << " MB" << endl;
    
    return 0;
}    
