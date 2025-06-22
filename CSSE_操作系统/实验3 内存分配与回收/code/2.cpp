#include <iostream>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <sys/resource.h>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// 读取/proc/pid/status中的VmSize
size_t getVmSize(pid_t pid) {
    string path = "/proc/" + to_string(pid) + "/status";
    ifstream file(path);
    if (!file.is_open()) return 0;
    
    string line;
    while (getline(file, line)) {
        if (line.find("VmSize:") != string::npos) {
            size_t pos = line.find("VmSize:") + 8;
            while (pos < line.size() && isspace(line[pos])) pos++;
            size_t value = 0;
            while (pos < line.size() && isdigit(line[pos])) {
                value = value * 10 + (line[pos] - '0');
                pos++;
            }
            return value; // 返回KB为单位的值
        }
    }
    return 0;
}

// 尝试分配指定大小的内存
bool allocateMemory(vector<char*>& blocks, size_t blockSizeMB) {
    size_t blockSize = blockSizeMB * 1024 * 1024; // 转换为字节
    try {
        char* block = new char[blockSize];
        // 写入数据以确保内存被实际分配
        for (size_t i = 0; i < blockSize; i += 4096) {
            block[i] = 'A';
        }
        blocks.push_back(block);
        return true;
    } catch (const bad_alloc& e) {
        return false;
    }
}

// 释放所有分配的内存
void freeAllMemory(vector<char*>& blocks) {
    for (char* block : blocks) {
        delete[] block;
    }
    blocks.clear();
}

int main() {
    pid_t pid = getpid();
    cout << "进程ID: " << pid << endl;
    
    // 获取系统页面大小
    long pageSize = sysconf(_SC_PAGESIZE);
    cout << "系统页面大小: " << pageSize << " 字节" << endl;
    
    // 获取RLIMIT_AS限制（软限制和硬限制）
    rlimit limit;
    if (getrlimit(RLIMIT_AS, &limit) == 0) {
        cout << "RLIMIT_AS 限制:" << endl;
        cout << "  软限制: " << (limit.rlim_cur == RLIM_INFINITY ? "无限制" : to_string(limit.rlim_cur / (1024 * 1024)) + " MB") << endl;
        cout << "  硬限制: " << (limit.rlim_max == RLIM_INFINITY ? "无限制" : to_string(limit.rlim_max / (1024 * 1024)) + " MB") << endl;
    } else {
        cout << "无法获取 RLIMIT_AS 限制" << endl;
    }
    
    vector<char*> blocks;
    size_t totalAllocatedMB = 0;
    size_t initialVmSize = getVmSize(pid);
    size_t stepSizeMB = 1024; // 初始分配步长为1GB
    
    cout << "\n开始测试最大虚拟内存分配..." << endl;
    cout << fixed << setprecision(2);
    
    while (true) {
        size_t currentVmSize = getVmSize(pid);
        cout << "\r已分配: " << totalAllocatedMB << " MB, 当前VmSize: " 
             << (currentVmSize / 1024.0) << " GB         ";
        cout.flush();
        
        // 尝试分配内存
        if (!allocateMemory(blocks, stepSizeMB)) {
            // 分配失败，减小步长重试
            if (stepSizeMB > 1) {
                stepSizeMB /= 2;
                cout << "\n分配失败，减小步长到 " << stepSizeMB << " MB" << endl;
            } else {
                // 无法再减小步长，测试结束
                cout << "\n\n测试结束" << endl;
                break;
            }
        } else {
            totalAllocatedMB += stepSizeMB;
        }
    }
    
    size_t finalVmSize = getVmSize(pid);
    cout << "初始 VmSize: " << (initialVmSize / 1024.0) << " GB" << endl;
    cout << "最终 VmSize: " << (finalVmSize / 1024.0) << " GB" << endl;
    cout << "成功分配的最大虚拟内存: " << totalAllocatedMB << " MB" << endl;
    cout << "进程最大虚拟内存限制估计: " << (finalVmSize - initialVmSize) / 1024.0 << " GB" << endl;
    
    // 释放所有内存
    freeAllMemory(blocks);
    cout << "已释放所有分配的内存" << endl;
    
    return 0;
}    
