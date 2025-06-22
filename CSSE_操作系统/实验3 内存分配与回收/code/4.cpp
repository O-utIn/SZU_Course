#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/resource.h>
#include <iomanip>
#include <cstring>
#include <thread>
#include <atomic>

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
void waitForInput(const string& message) {
    cout << "\n" << message << " 按Enter继续...";
    cin.ignore();
}

int main() {
    pid_t pid = getpid();
    cout << "本进程ID: " << pid << endl;
    
    // 获取系统物理内存总量
    ifstream meminfo("/proc/meminfo");
    string line;
    size_t totalMemKB = 0;
    while (getline(meminfo, line)) {
        if (line.find("MemTotal:") != string::npos) {
            sscanf(line.c_str(), "MemTotal:\t%zu kB", &totalMemKB);
            break;
        }
    }
    meminfo.close();
    
    if (totalMemKB == 0) {
        cerr << "无法获取系统内存信息" << endl;
        return 1;
    }
    
    cout << "系统总内存: " << totalMemKB / 1024.0 << " MB" << endl;
    
    // 计算要分配的内存大小（略大于系统总内存）
    size_t allocateSizeKB = totalMemKB * 1.1; // 分配110%的系统内存
    size_t allocateSizeMB = allocateSizeKB / 1024;
    size_t allocateSizeBytes = allocateSizeKB * 1024;
    
    cout << "计划分配内存: " << allocateSizeMB << " MB" << endl;
    
    // 初始内存状态
    MemoryStatus initialStatus = getMemoryStatus(pid);
    printMemoryStatus(initialStatus, "初始内存状态");
    waitForInput("请在按Enter前确保上一个程序处于运行状态");
    
    // 分配内存
    vector<char*> memoryChunks;
    const size_t chunkSize = 128 * 1024 * 1024; // 128MB每块
    atomic<bool> keepRunning(true);
    
    // 启动监控线程
    thread monitor([pid, &keepRunning]() {
        while (keepRunning) {
            MemoryStatus status = getMemoryStatus(pid);
            cout << "\r当前VmRSS: " << status.vmRSS / 1024.0 << " MB         ";
            cout.flush();
            this_thread::sleep_for(chrono::seconds(1));
        }
    });
    
    try {
        // 分块分配内存
        size_t allocatedBytes = 0;
        while (allocatedBytes < allocateSizeBytes) {
            size_t currentChunkSize = min(chunkSize, allocateSizeBytes - allocatedBytes);
            char* chunk = new char[currentChunkSize];
            memoryChunks.push_back(chunk);
            allocatedBytes += currentChunkSize;
            
            // 每隔4KB写入数据，强制物理内存分配
            for (size_t i = 0; i < currentChunkSize; i += 4096) {
                chunk[i] = 'A';
            }
            
            cout << "\r已分配: " << allocatedBytes / (1024 * 1024) << " MB / " 
                 << allocateSizeMB << " MB";
            cout.flush();
            
            // 短暂睡眠，避免过快分配导致系统崩溃
            this_thread::sleep_for(chrono::milliseconds(100));
        }
        
        cout << "\n内存分配完成，开始每隔4KB递增操作..." << endl;
        
        // 每隔4KB递增内存值，持续竞争内存
        while (true) {
            for (char* chunk : memoryChunks) {
                for (size_t i = 0; i < chunkSize; i += 4096) {
                    chunk[i]++;
                }
            }
            cout << "\r内存递增操作完成一次循环";
            cout.flush();
            this_thread::sleep_for(chrono::seconds(1));
        }
    } catch (const bad_alloc& e) {
        cout << "\n内存分配失败: " << e.what() << endl;
    } catch (...) {
        cout << "\n未知异常" << endl;
    }
    
    // 清理资源
    keepRunning = false;
    if (monitor.joinable()) {
        monitor.join();
    }
    
    for (char* chunk : memoryChunks) {
        delete[] chunk;
    }
    
    // 最终内存状态
    MemoryStatus finalStatus = getMemoryStatus(pid);
    printMemoryStatus(finalStatus, "释放内存后的内存状态");
    
    return 0;
}    
