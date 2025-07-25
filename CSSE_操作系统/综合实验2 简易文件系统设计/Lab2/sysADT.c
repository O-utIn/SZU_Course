#include <stdio.h>
#include <stdint.h>

// 使用 uint8_t 代替 byte
typedef uint8_t byte;

// 数据块结构体
typedef struct Data_block {
    byte data[4 * 1024];  // 每个数据块大小为 4KB
} Data_block;

// inode 结构体
typedef struct Inode {
    int inode_tag;        // 标记该 inode 是否使用
    int type;             // 类型，0 表示文件，1 表示目录
    int myIndex;          // 当前 inode 的编号
    char path[256];       // 文件路径
    char name[256];       // 文件名
    int size;             // 文件大小
    int start_data_index; // 文件使用的数据块起始索引
    int end_data_index;   // 文件使用的数据块结束索引
    char** son_namelist;  // 子文件或目录名（用于目录）
    int* son_index_list;  // 子节点 inode 索引
    int list_lastpoint;   // 指向最后一个子项的位置
} Inode;

// 文件系统结构体
typedef struct fSystem {
    Inode i_bmap[200];    // inode 表，共 200 个
    int d_bmap[25344];    // 数据块位图，共 25344 个数据块
    Data_block data_region[25344]; // 数据块区域
} fSystem;

int main() {
    // 输出 Inode 结构体大小
    printf("Inode size: %lu bytes\n", sizeof(Inode));

    // 输出 Data_block 结构体大小
    printf("Data_block size: %lu bytes\n", sizeof(Data_block));

    // 输出整个 fSystem 文件系统结构体的大小（以 MB 为单位）
    printf("fSystem size: %.6f MB\n", (double)sizeof(fSystem) / (1024 * 1024));

    return 0;
}

