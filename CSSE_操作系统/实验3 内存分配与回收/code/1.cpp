#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/resource.h>
#include <iomanip>

using namespace std;

// 内存块结构
struct MemoryBlock {
    char* pointer;
    size_t size;
    bool allocated;
    char pattern;  // 用于标记内存块的字符模式
};

// 读取/proc/pid/status文件中的内存信息
void printMemoryStatus(pid_t pid) {
    string statusPath = "/proc/" + to_string(pid) + "/status";
    ifstream statusFile(statusPath);
    
    if (statusFile.is_open()) {
        string line;
        vector<string> fields = {"VmSize:", "VmRSS:", "VmData:", "VmHWM:"};
        
        cout << "\n=== 内存状态 ===" << endl;
        while (getline(statusFile, line)) {
            for (const auto& field : fields) {
                if (line.find(field) != string::npos) {
                    cout << line << endl;
                }
            }
        }
        statusFile.close();
    } else {
        cerr << "无法打开 " << statusPath << endl;
    }
}

// 读取/proc/pid/maps文件并高亮显示我们分配的内存块
void printMemoryMaps(pid_t pid, const vector<MemoryBlock>& blocks) {
    string mapsPath = "/proc/" + to_string(pid) + "/maps";
    ifstream mapsFile(mapsPath);
    
    if (mapsFile.is_open()) {
        string line;
        cout << "\n=== 内存映射 ===" << endl;
        
        // 为每个内存块创建一个查找表
        vector<pair<void*, void*>> blockRanges;
        for (const auto& block : blocks) {
            if (block.allocated) {
                blockRanges.push_back({block.pointer, block.pointer + block.size});
            }
        }
        
        // 打印并高亮显示我们分配的内存块
        while (getline(mapsFile, line)) {
            // 解析地址范围
            size_t dashPos = line.find('-');
            if (dashPos != string::npos) {
                string startAddrStr = line.substr(0, dashPos);
                string endAddrStr = line.substr(dashPos + 1, line.find(' ', dashPos) - dashPos - 1);
                
                // 将地址转换为数值
                uintptr_t startAddr = stoull(startAddrStr, nullptr, 16);
                uintptr_t endAddr = stoull(endAddrStr, nullptr, 16);
                
                // 检查是否与我们分配的内存块重叠
                bool isOurBlock = false;
                for (const auto& range : blockRanges) {
                    uintptr_t blockStart = reinterpret_cast<uintptr_t>(range.first);
                    uintptr_t blockEnd = reinterpret_cast<uintptr_t>(range.second);
                    
                    // 如果映射与我们的内存块有重叠
                    if (startAddr < blockEnd && endAddr > blockStart) {
                        isOurBlock = true;
                        break;
                    }
                }
                
                // 高亮显示我们分配的内存块
                if (isOurBlock) {
                    cout << "\033[1;32m" << line << "\033[0m" << endl;  // 绿色高亮
                } else {
                    cout << line << endl;
                }
            } else {
                cout << line << endl;
            }
        }
        mapsFile.close();
    } else {
        cerr << "无法打开 " << mapsPath << endl;
    }
}

// 等待用户输入继续
void waitForInput(const string& message) {
    cout << "\n" << message << " 按Enter继续...";
    cin.ignore();
}

// 填充内存块，使用特定模式以便于识别
void fillMemoryBlock(MemoryBlock& block) {
    for (size_t i = 0; i < block.size; i++) {
        block.pointer[i] = block.pattern;
    }
}

int main() {
    pid_t pid = getpid();
    cout << "进程ID: " << pid << endl;
    
    // 初始状态
    cout << "\n=== 初始状态 ===" << endl;
    printMemoryStatus(pid);
    printMemoryMaps(pid, {});
    waitForInput("初始状态已记录");
    
    // 分配6个128MB内存块
    const size_t blockSize = 128 * 1024 * 1024; // 128MB
    vector<MemoryBlock> blocks(6);
    
    cout << "\n=== 分配6个128MB内存块 ===" << endl;
    for (int i = 0; i < 6; ++i) {
        blocks[i].size = blockSize;
        blocks[i].pattern = '1' + i;  // 使用不同的字符模式 '1' 到 '6'
        blocks[i].pointer = new char[blockSize];
        blocks[i].allocated = true;
        
        // 填充内存块以便于识别
        fillMemoryBlock(blocks[i]);
        
        cout << "已分配块 " << (i + 1) << " (128MB, 模式: " << blocks[i].pattern << ")" << endl;
        printMemoryStatus(pid);
        printMemoryMaps(pid, blocks);
        waitForInput("块 " + to_string(i + 1) + " 已分配");
    }
    
    // 释放第2、3、5号内存块
    cout << "\n=== 释放第2、3、5号内存块 ===" << endl;
    for (int i : {1, 2, 4}) { // 索引从0开始
        delete[] blocks[i].pointer;
        blocks[i].allocated = false;
        cout << "已释放块 " << (i + 1) << " (128MB)" << endl;
        printMemoryStatus(pid);
        printMemoryMaps(pid, blocks);
        waitForInput("块 " + to_string(i + 1) + " 已释放");
    }
    
    // 分配1024MB内存
    cout << "\n=== 分配1024MB内存 ===" << endl;
    MemoryBlock largeBlock;
    largeBlock.size = 1024 * 1024 * 1024; // 1024MB
    largeBlock.pattern = 'L';
    largeBlock.pointer = new char[largeBlock.size];
    largeBlock.allocated = true;
    
    // 填充内存块
    fillMemoryBlock(largeBlock);
    
    cout << "已分配1024MB内存块 (模式: L)" << endl;
    printMemoryStatus(pid);
    printMemoryMaps(pid, {largeBlock});
    waitForInput("1024MB块已分配");
    
    // 尝试分配64MB内存
    cout << "\n=== 尝试分配64MB内存 ===" << endl;
    MemoryBlock smallBlock;
    smallBlock.size = 64 * 1024 * 1024; // 64MB
    smallBlock.pattern = 'S';
    smallBlock.pointer = new char[smallBlock.size];
    smallBlock.allocated = true;
    
    // 填充内存块
    fillMemoryBlock(smallBlock);
    
    cout << "已分配64MB内存块 (模式: S)" << endl;
    printMemoryStatus(pid);
    printMemoryMaps(pid, {smallBlock});
    waitForInput("64MB块已分配");
    
    // 释放所有内存
    cout << "\n=== 释放所有内存 ===" << endl;
    for (auto& block : blocks) {
        if (block.allocated) {
            delete[] block.pointer;
        }
    }
    if (largeBlock.allocated) {
        delete[] largeBlock.pointer;
    }
    if (smallBlock.allocated) {
        delete[] smallBlock.pointer;
    }
    
    cout << "所有内存已释放" << endl;
    printMemoryStatus(pid);
    printMemoryMaps(pid, {});
    
    return 0;
}    
