#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BLOCK_SIZE 4096
#define SHM_KEY 0x123456      // 共享内存键值
#define SHM_SIZE (100 * 1024 * 1024)  // 100MB
#define READ_DELAY 2          // 模拟读操作耗时(秒)
#define WRITE_DELAY 3         // 模拟写操作耗时(秒)

typedef struct Data_block {
    char data[BLOCK_SIZE];
} Data_block;

typedef struct Inode {
    int inode_tag;
    int type;  // 0 for file, 1 for directory
    int myIndex;
    char path[256];
    char name[128];
    int size;
    int start_data_index;
    int end_data_index;
    int parent_index;
    char** son_namelist;
    int* son_index_list;
    int list_lastpoint;
    
    // 信号量相关成员
    int read_count;         // 读者计数
    sem_t rw_mutex;         // 读写互斥锁
    sem_t mutex;            // 保护read_count的锁
    sem_t fair_queue;       // 公平队列锁
} Inode;

typedef struct fSystem {
    Inode inodes[200];    // inode 表，共 200 个
    int d_bmap[25344];    // 数据块位图，共 25344 个数据块
    Data_block data_region[25344]; // 数据块区域
    char current_paths[10][256];    // 支持10个终端的当前路径
    int path_count;                // 当前路径索引计数
} fSystem;

void* shared_memory = NULL;
fSystem* fs = NULL;  // 指向文件系统的指针
Inode* root_dir = NULL;
int terminal_id = -1;  // 当前终端ID

// 获取当前终端的路径
char* get_current_path() {
    if (terminal_id == -1) {
        // 分配新的终端ID和路径
        if (fs->path_count < 10) {
            terminal_id = fs->path_count++;
            strcpy(fs->current_paths[terminal_id], "/");
        } else {
            printf("Error: Too many terminals\n");
            exit(1);
        }
    }
    return fs->current_paths[terminal_id];
}

// 设置当前终端的路径
void set_current_path(const char* path) {
    if (terminal_id == -1) get_current_path();
    strcpy(fs->current_paths[terminal_id], path);
}

// 分配一个空闲的数据块
int alloc_data_block() {
    for (int i = 0; i < 25344; i++) {
        if (fs->d_bmap[i] == 0) {
            fs->d_bmap[i] = 1;
            return i;
        }
    }
    return -1; // 没有找到空闲的数据块
}

// 释放一个数据块
void free_data_block(int index) {
    if (index >= 0 && index < 25344) {
        fs->d_bmap[index] = 0;
    }
}

// 释放连续的数据块范围
void free_data_blocks(int start, int end) {
    for (int i = start; i <= end && i < 25344; i++) {
        free_data_block(i);
    }
}

// 为子目录/文件列表分配内存
void allocate_son_lists(Inode* node) {
    if (node->son_namelist == NULL) {
        node->son_namelist = (char**)malloc(sizeof(char*) * 100);
        node->son_index_list = (int*)malloc(sizeof(int) * 100);
        if (!node->son_namelist || !node->son_index_list) {
            printf("Error: Memory allocation failed\n");
            exit(1);
        }
        node->list_lastpoint = -1;
    }
}

// 将子目录/文件添加到父目录列表
void add_son_to_parent(Inode* parent, Inode* son) {
    if (parent == NULL || son == NULL) return;
    
    allocate_son_lists(parent);
    parent->list_lastpoint++;
    parent->son_namelist[parent->list_lastpoint] = (char*)malloc(strlen(son->name) + 1);
    if (!parent->son_namelist[parent->list_lastpoint]) {
        printf("Error: Memory allocation failed\n");
        exit(1);
    }
    strcpy(parent->son_namelist[parent->list_lastpoint], son->name);
    parent->son_index_list[parent->list_lastpoint] = son->myIndex;
}

// 从父目录列表中移除子项
void remove_son_from_parent(Inode* parent, int son_index) {
    if (parent == NULL || parent->list_lastpoint < 0) return;
    
    for (int i = 0; i <= parent->list_lastpoint; i++) {
        if (parent->son_index_list[i] == son_index) {
            free(parent->son_namelist[i]);
            // 将后面的元素前移
            for (int j = i; j < parent->list_lastpoint; j++) {
                parent->son_namelist[j] = parent->son_namelist[j + 1];
                parent->son_index_list[j] = parent->son_index_list[j + 1];
            }
            parent->list_lastpoint--;
            break;
        }
    }
}

// 查找inode
Inode* find_inode(const char* path) {
    // 处理相对路径
    char abs_path[256];
    if (path[0] != '/') {
        if (strcmp(get_current_path(), "/") == 0) {
            sprintf(abs_path, "/%s", path);
        } else {
            sprintf(abs_path, "%s/%s", get_current_path(), path);
        }
    } else {
        strcpy(abs_path, path);
    }
    
    for (int i = 0; i < 200; i++) {
        if (fs->inodes[i].inode_tag == 1 && 
            strcmp(fs->inodes[i].path, abs_path) == 0) {
            return &fs->inodes[i];
        }
    }
    return NULL;
}

// 获取父目录路径
void get_parent_path(const char* path, char* parent_path) {
    const char* last_slash = strrchr(path, '/');
    if (last_slash == path) {
        // 根目录的父目录是自身
        strcpy(parent_path, "/");
    } else if (last_slash != NULL) {
        // 复制直到最后一个斜杠
        strncpy(parent_path, path, last_slash - path);
        parent_path[last_slash - path] = '\0';
        if (strlen(parent_path) == 0) strcpy(parent_path, "/");
    } else {
        // 没有斜杠，默认为根目录
        strcpy(parent_path, "/");
    }
}

// 获取路径中的名称部分
const char* get_name_from_path(const char* path) {
    const char* last_slash = strrchr(path, '/');
    if (last_slash == NULL) return path;
    return last_slash + 1;
}

// 规范化路径，处理.和..
void normalize_path(const char* path, char* normalized) {
    char parts[256][128];
    int count = 0;
    
    // 分割路径
    char temp[256];
    strcpy(temp, path);
    
    char* token = strtok(temp, "/");
    while (token != NULL) {
        if (strcmp(token, "") == 0 || strcmp(token, ".") == 0) {
            // 忽略空部分和.
        } else if (strcmp(token, "..") == 0) {
            // 回退一级
            if (count > 0) {
                count--;
            }
        } else {
            // 有效部分
            strcpy(parts[count], token);
            count++;
        }
        token = strtok(NULL, "/");
    }
    
    // 构建规范化路径
    if (count == 0) {
        strcpy(normalized, "/");
    } else {
        normalized[0] = '\0';
        for (int i = 0; i < count; i++) {
            strcat(normalized, "/");
            strcat(normalized, parts[i]);
        }
    }
}

// 解析路径，返回绝对路径
void resolve_path(const char* path, char* resolved) {
    if (path[0] == '/') {
        // 绝对路径
        normalize_path(path, resolved);
    } else {
        // 相对路径
        char temp[256];
        if (strcmp(get_current_path(), "/") == 0) {
            sprintf(temp, "/%s", path);
        } else {
            sprintf(temp, "%s/%s", get_current_path(), path);
        }
        normalize_path(temp, resolved);
    }
}

// 初始化Inode信号量
void init_inode_sem(Inode* node) {
    node->read_count = 0;
    sem_init(&node->rw_mutex, 1, 1);   // 进程间共享的读写互斥锁
    sem_init(&node->mutex, 1, 1);      // 进程间共享的保护read_count的锁
    sem_init(&node->fair_queue, 1, 1); // 进程间共享的公平队列锁
}

// 创建目录
int mkdir(const char* path) {
    // 解析路径
    char resolved[256];
    resolve_path(path, resolved);
    
    // 检查路径是否已存在
    if (find_inode(resolved) != NULL) {
        printf("Error: Directory '%s' already exists\n", resolved);
        return -1;
    }
    
    // 获取父目录路径
    char parent_path[256];
    get_parent_path(resolved, parent_path);
    
    // 查找父目录
    Inode* parent = find_inode(parent_path);
    if (parent == NULL) {
        printf("Error: Parent directory '%s' not found\n", parent_path);
        return -1;
    }
    
    // 确保父目录是目录类型
    if (parent->type != 1) {
        printf("Error: Parent '%s' is not a directory\n", parent_path);
        return -1;
    }
    
    // 在 inode 表中找到一个空闲的 inode 项
    int i;
    for (i = 0; i < 200; i++) {
        if (fs->inodes[i].inode_tag == 0) {
            break;
        }
    }
    if (i == 200) {
        printf("Error: Inode table is full\n");
        return -1;
    }
    
    // 初始化新的 inode 项
    Inode* new_dir = &fs->inodes[i];
    new_dir->inode_tag = 1;
    new_dir->type = 1;  // 目录类型
    new_dir->myIndex = i;
    new_dir->parent_index = parent->myIndex;
    strcpy(new_dir->path, resolved);
    strcpy(new_dir->name, get_name_from_path(resolved));
    new_dir->size = 0;
    new_dir->start_data_index = -1;
    new_dir->end_data_index = -1;
    new_dir->son_namelist = NULL;
    new_dir->son_index_list = NULL;
    new_dir->list_lastpoint = -1;
    
    // 初始化信号量
    init_inode_sem(new_dir);
    
    // 将新目录添加到父目录
    add_son_to_parent(parent, new_dir);
    
    printf("Directory '%s' created\n", resolved);
    return 0;
}

// 删除目录
int rmdir(const char* path) {
    // 解析路径
    char resolved[256];
    resolve_path(path, resolved);
    
    // 查找目录
    Inode* dir = find_inode(resolved);
    if (dir == NULL) {
        printf("Error: Directory '%s' not found\n", resolved);
        return -1;
    }
    
    // 确保是目录类型
    if (dir->type != 1) {
        printf("Error: '%s' is not a directory\n", resolved);
        return -1;
    }
    
    // 检查目录是否为空
    if (dir->list_lastpoint >= 0) {
        printf("Error: Directory '%s' is not empty\n", resolved);
        return -1;
    }
    
    // 仅阻止删除当前目录本身
    if (strcmp(resolved, get_current_path()) == 0) {
        printf("Error: Cannot remove current directory\n");
        return -1;
    }
    
    // 从父目录中移除
    Inode* parent = &fs->inodes[dir->parent_index];
    remove_son_from_parent(parent, dir->myIndex);
    
    // 释放inode
    dir->inode_tag = 0;
    printf("Directory '%s' removed\n", resolved);
    return 0;
}

// 创建文件 (使用open命令)
int open_file(const char* path) {
    // 解析路径
    char resolved[256];
    resolve_path(path, resolved);
    
    // 检查文件是否已存在
    if (find_inode(resolved) != NULL) {
        printf("Error: File '%s' already exists\n", resolved);
        return -1;
    }
    
    // 获取父目录路径
    char parent_path[256];
    get_parent_path(resolved, parent_path);
    
    // 查找父目录
    Inode* parent = find_inode(parent_path);
    if (parent == NULL) {
        printf("Error: Parent directory '%s' not found\n", parent_path);
        return -1;
    }
    
    // 确保父目录是目录类型
    if (parent->type != 1) {
        printf("Error: Parent '%s' is not a directory\n", parent_path);
        return -1;
    }
    
    // 在 inode 表中找到一个空闲的 inode 项
    int i;
    for (i = 0; i < 200; i++) {
        if (fs->inodes[i].inode_tag == 0) {
            break;
        }
    }
    if (i == 200) {
        printf("Error: Inode table is full\n");
        return -1;
    }
    
    // 初始化新的 inode 项
    Inode* new_file = &fs->inodes[i];
    new_file->inode_tag = 1;
    new_file->type = 0;  // 文件类型
    new_file->myIndex = i;
    new_file->parent_index = parent->myIndex;
    strcpy(new_file->path, resolved);
    strcpy(new_file->name, get_name_from_path(resolved));
    new_file->size = 0;
    new_file->start_data_index = -1;
    new_file->end_data_index = -1;
    new_file->son_namelist = NULL;
    new_file->son_index_list = NULL;
    new_file->list_lastpoint = -1;
    
    // 初始化信号量
    init_inode_sem(new_file);
    
    // 将新文件添加到父目录
    add_son_to_parent(parent, new_file);
    
    printf("File '%s' created\n", resolved);
    return 0;
}

// 删除文件
int rm(const char* path) {
    // 解析路径
    char resolved[256];
    resolve_path(path, resolved);
    
    // 查找文件或目录
    Inode* node = find_inode(resolved);
    if (node == NULL) {
        printf("Error: File or directory '%s' not found\n", resolved);
        return -1;
    }
    
    // 如果是目录，需要检查是否为空
    if (node->type == 1) {
        if (node->list_lastpoint >= 0) {
            printf("Error: Directory '%s' is not empty\n", resolved);
            return -1;
        }
        
        // 仅阻止删除当前目录本身
        if (strcmp(resolved, get_current_path()) == 0) {
            printf("Error: Cannot remove current directory\n");
            return -1;
        }
    } else {
        // 如果是文件，释放数据块
        if (node->start_data_index != -1 && node->end_data_index != -1) {
            free_data_blocks(node->start_data_index, node->end_data_index);
        }
    }
    
    // 从父目录中移除
    Inode* parent = &fs->inodes[node->parent_index];
    remove_son_from_parent(parent, node->myIndex);
    
    // 释放inode
    node->inode_tag = 0;
    printf("'%s' removed\n", resolved);
    return 0;
}

// 切换目录
int cd(const char* path) {
    // 解析路径
    char resolved[256];
    resolve_path(path, resolved);
    
    // 查找目录
    Inode* dir = find_inode(resolved);
    if (dir == NULL) {
        printf("Error: Directory '%s' not found\n", resolved);
        return -1;
    }
    
    // 确保是目录类型
    if (dir->type != 1) {
        printf("Error: '%s' is not a directory\n", resolved);
        return -1;
    }
    
    // 更新当前路径
    set_current_path(resolved);
    return 0;
}

// 列出目录内容
void ls(const char* path) {
    // 解析路径
    char resolved[256];
    if (path == NULL || strlen(path) == 0) {
        strcpy(resolved, get_current_path());
    } else {
        resolve_path(path, resolved);
    }
    
    // 查找目录
    Inode* dir = find_inode(resolved);
    if (dir == NULL) {
        printf("Error: Directory '%s' not found\n", resolved);
        return;
    }
    
    // 确保是目录类型
    if (dir->type != 1) {
        printf("Error: '%s' is not a directory\n", resolved);
        return;
    }
    
    // 读者获取锁
    sem_wait(&dir->fair_queue);
    printf("Terminal %d: Acquired fair_queue for ls (directory)\n", terminal_id);
    
    sem_wait(&dir->mutex);
    printf("Terminal %d: Acquired mutex for ls (directory)\n", terminal_id);
    
    dir->read_count++;
    if (dir->read_count == 1) {
        sem_wait(&dir->rw_mutex);
        printf("Terminal %d: First reader acquired rw_mutex for ls (directory)\n", terminal_id);
    }
    
    sem_post(&dir->mutex);
    sem_post(&dir->fair_queue);
    
    // 输出目录内容
    printf("Contents of directory '%s':\n", resolved);
    for (int i = 0; i <= dir->list_lastpoint; i++) {
        if (dir->son_namelist[i] != NULL) {
            printf("  %s\n", dir->son_namelist[i]);
        }
    }
    
    // 模拟读操作耗时
    printf("Terminal %d: Simulating directory read delay...\n", terminal_id);
    sleep(READ_DELAY);
    
    // 读者释放锁
    sem_wait(&dir->mutex);
    printf("Terminal %d: Acquired mutex to release lock for ls (directory)\n", terminal_id);
    
    dir->read_count--;
    if (dir->read_count == 0) {
        sem_post(&dir->rw_mutex);
        printf("Terminal %d: Last reader released rw_mutex for ls (directory)\n", terminal_id);
    }
    
    sem_post(&dir->mutex);
    printf("Terminal %d: Released mutex for ls (directory)\n", terminal_id);
}

// 读取文件（带信号量控制）
int read_file(const char* path) {
    char resolved[256];
    resolve_path(path, resolved);
    Inode* file = find_inode(resolved);
    if (!file || file->type != 0) {
        printf("Error: File not found or not a file\n");
        return -1;
    }

    // 读者逻辑：公平排队 -> 保护读计数 -> 第一个读者加写锁
    printf("Terminal %d: Requesting read lock on %s\n", terminal_id, resolved);
    sem_wait(&file->fair_queue);
    printf("Terminal %d: Acquired fair_queue for read\n", terminal_id);
    
    sem_wait(&file->mutex);
    printf("Terminal %d: Acquired mutex for read\n", terminal_id);
    
    file->read_count++;
    if (file->read_count == 1) {
        sem_wait(&file->rw_mutex);
        printf("Terminal %d: First reader acquired rw_mutex for read\n", terminal_id);
    }
    
    printf("Terminal %d: Current reader count: %d\n", terminal_id, file->read_count);
    
    sem_post(&file->mutex);
    sem_post(&file->fair_queue);
    printf("Terminal %d: Released mutex and fair_queue for read\n", terminal_id);

    // 读取文件内容
    printf("Terminal %d: Reading content of %s:\n", terminal_id, resolved);
    for (int i = 0; i < file->size; i++) {
        int block_num = i / BLOCK_SIZE;
        int offset = i % BLOCK_SIZE;
        putchar(fs->data_region[file->start_data_index + block_num].data[offset]);
    }
    printf("\n");
    
    // 模拟读操作耗时
    printf("Terminal %d: Simulating read delay...\n", terminal_id);
    sleep(READ_DELAY);

    // 读者结束：最后一个读者释放写锁
    printf("Terminal %d: Releasing read lock on %s\n", terminal_id, resolved);
    sem_wait(&file->mutex);
    printf("Terminal %d: Acquired mutex to release read lock\n", terminal_id);
    
    file->read_count--;
    printf("Terminal %d: Current reader count after release: %d\n", terminal_id, file->read_count);
    
    if (file->read_count == 0) {
        sem_post(&file->rw_mutex);
        printf("Terminal %d: Last reader released rw_mutex for read\n", terminal_id);
    }
    
    sem_post(&file->mutex);
    printf("Terminal %d: Released mutex after read\n", terminal_id);

    return 0;
}

// 写入文件（带信号量控制）
int write_file(const char* path, const char* content, int size, int append) {
    char resolved[256];
    resolve_path(path, resolved);
    Inode* file = find_inode(resolved);
    if (!file || file->type != 0) {
        printf("Error: File not found or not a file\n");
        return -1;
    }

    // 写者逻辑：公平排队 -> 独占写锁
    printf("Terminal %d: Requesting write lock on %s\n", terminal_id, resolved);
    sem_wait(&file->fair_queue);
    printf("Terminal %d: Acquired fair_queue for write\n", terminal_id);
    
    sem_wait(&file->rw_mutex);
    printf("Terminal %d: Acquired rw_mutex for write (exclusive lock)\n", terminal_id);
    
    sem_post(&file->fair_queue);
    printf("Terminal %d: Released fair_queue for write\n", terminal_id);
    
    printf("Terminal %d: Starting write operation on %s\n", terminal_id, resolved);

    // 写入文件内容
    if (append && file->size > 0) {
        // 追加模式：保留原有内容，在末尾添加新内容
        char* buffer = (char*)malloc(file->size + size);
        if (!buffer) {
            printf("Error: Memory allocation failed\n");
            sem_post(&file->rw_mutex);
            return -1;
        }
        
        // 读取原有内容
        for (int i = 0; i < file->size; i++) {
            int block_num = i / BLOCK_SIZE;
            int offset = i % BLOCK_SIZE;
            buffer[i] = fs->data_region[file->start_data_index + block_num].data[offset];
        }
        
        // 复制新内容
        for (int i = 0; i < size; i++) {
            buffer[file->size + i] = content[i];
        }
        
        int total_size = file->size + size;
        
        // 释放旧的数据块
        if (file->start_data_index != -1 && file->end_data_index != -1) {
            free_data_blocks(file->start_data_index, file->end_data_index);
        }
        
        // 计算需要的数据块数量
        int blocks_needed = (total_size + BLOCK_SIZE - 1) / BLOCK_SIZE;
        if (blocks_needed == 0) {
            file->size = 0;
            file->start_data_index = -1;
            file->end_data_index = -1;
            free(buffer);
            sem_post(&file->rw_mutex);
            return 0;
        }
        
        // 分配新的数据块
        int start_block = -1;
        int consecutive = 0;
        
        // 查找连续的空闲块
        for (int i = 0; i < 25344; i++) {
            if (fs->d_bmap[i] == 0) {
                if (start_block == -1) {
                    start_block = i;
                }
                consecutive++;
                
                if (consecutive == blocks_needed) {
                    // 找到足够的连续块
                    for (int j = start_block; j < start_block + blocks_needed; j++) {
                        fs->d_bmap[j] = 1;
                    }
                    file->start_data_index = start_block;
                    file->end_data_index = start_block + blocks_needed - 1;
                    file->size = total_size;
                    break;
                }
            } else {
                // 重置计数
                start_block = -1;
                consecutive = 0;
            }
        }
        
        if (file->start_data_index == -1) {
            printf("Error: Not enough contiguous space for file\n");
            free(buffer);
            sem_post(&file->rw_mutex);
            return -1;
        }
        
        // 写入合并后的内容
        for (int i = 0; i < total_size; i++) {
            int block_num = i / BLOCK_SIZE;
            int offset = i % BLOCK_SIZE;
            fs->data_region[file->start_data_index + block_num].data[offset] = buffer[i];
        }
        
        free(buffer);
    } else {
        // 覆盖模式：直接写入新内容
        // 如果文件大小发生变化，需要重新分配数据块
        if (size != file->size) {
            // 释放旧的数据块
            if (file->start_data_index != -1 && file->end_data_index != -1) {
                free_data_blocks(file->start_data_index, file->end_data_index);
            }
            
            // 计算需要的数据块数量
            int blocks_needed = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
            if (blocks_needed == 0) {
                file->size = 0;
                file->start_data_index = -1;
                file->end_data_index = -1;
                sem_post(&file->rw_mutex);
                return 0;
            }
            
            // 分配新的数据块
            int start_block = -1;
            int consecutive = 0;
            
            // 查找连续的空闲块
            for (int i = 0; i < 25344; i++) {
                if (fs->d_bmap[i] == 0) {
                    if (start_block == -1) {
                        start_block = i;
                    }
                    consecutive++;
                    
                    if (consecutive == blocks_needed) {
                        // 找到足够的连续块
                        for (int j = start_block; j < start_block + blocks_needed; j++) {
                            fs->d_bmap[j] = 1;
                        }
                        file->start_data_index = start_block;
                        file->end_data_index = start_block + blocks_needed - 1;
                        file->size = size;
                        break;
                    }
                } else {
                    // 重置计数
                    start_block = -1;
                    consecutive = 0;
                }
            }
            
            if (file->start_data_index == -1) {
                printf("Error: Not enough contiguous space for file\n");
                sem_post(&file->rw_mutex);
                return -1;
            }
        }
        
        // 写入数据
        if (size > 0 && file->start_data_index != -1) {
            for (int i = 0; i < size; i++) {
                int block_num = i / BLOCK_SIZE;
                int offset = i % BLOCK_SIZE;
                fs->data_region[file->start_data_index + block_num].data[offset] = content[i];
            }
        }
    }
    
    // 模拟写操作耗时
    printf("Terminal %d: Simulating write delay...\n", terminal_id);
    sleep(WRITE_DELAY);
    
    printf("Terminal %d: File %s written\n", terminal_id, resolved);
    sem_post(&file->rw_mutex);  // 释放写锁
    printf("Terminal %d: Released rw_mutex for write\n", terminal_id);
    return 0;
}

// 重命名文件或目录
int my_rename(const char* old_path, const char* new_path) {
    // 解析路径
    char resolved_old[256];
    char resolved_new[256];
    resolve_path(old_path, resolved_old);
    resolve_path(new_path, resolved_new);
    
    // 查找源文件/目录
    Inode* node = find_inode(resolved_old);
    if (node == NULL) {
        printf("Error: '%s' not found\n", resolved_old);
        return -1;
    }
    
    // 检查目标路径是否已存在
    if (find_inode(resolved_new) != NULL) {
        printf("Error: '%s' already exists\n", resolved_new);
        return -1;
    }
    
    // 获取新的父目录路径和名称
    char new_parent_path[256];
    get_parent_path(resolved_new, new_parent_path);
    const char* new_name = get_name_from_path(resolved_new);
    
    // 查找新的父目录
    Inode* new_parent = find_inode(new_parent_path);
    if (new_parent == NULL) {
        printf("Error: Parent directory '%s' not found\n", new_parent_path);
        return -1;
    }
    
    // 如果是移动到不同的父目录，需要更新父子关系
    if (node->parent_index != new_parent->myIndex) {
        // 从旧父目录中移除
        Inode* old_parent = &fs->inodes[node->parent_index];
        remove_son_from_parent(old_parent, node->myIndex);
        
        // 添加到新父目录
        node->parent_index = new_parent->myIndex;
        add_son_to_parent(new_parent, node);
    } else {
        // 只重命名，更新父目录中的名称
        for (int i = 0; i <= new_parent->list_lastpoint; i++) {
            if (new_parent->son_index_list[i] == node->myIndex) {
                free(new_parent->son_namelist[i]);
                new_parent->son_namelist[i] = (char*)malloc(strlen(new_name) + 1);
                strcpy(new_parent->son_namelist[i], new_name);
                break;
            }
        }
    }
    
    // 更新inode路径和名称
    strcpy(node->path, resolved_new);
    strcpy(node->name, new_name);
    
    // 如果是目录，递归更新子项的路径
    if (node->type == 1) {
        for (int i = 0; i <= node->list_lastpoint; i++) {
            int child_index = node->son_index_list[i];
            Inode* child = &fs->inodes[child_index];
            
            char child_new_path[256];
            if (strcmp(resolved_new, "/") == 0) {
                sprintf(child_new_path, "/%s", child->name);
            } else {
                sprintf(child_new_path, "%s/%s", resolved_new, child->name);
            }
            
            // 递归更新子目录
            my_rename(child->path, child_new_path);
        }
    }
    
    printf("'%s' renamed to '%s'\n", resolved_old, resolved_new);
    return 0;
}

// 显示当前工作目录
void pwd() {
    printf("%s\n", get_current_path());
}

// 清理共享内存
void cleanup_shared_memory() {
    // 分离共享内存
    if (shmdt(shared_memory) == -1) {
        perror("shmdt failed");
    }
    
    // 仅在所有终端都退出时删除共享内存
    // 实际实现中需要引用计数或其他机制来判断
    // 这里简化处理，不删除共享内存，避免意外删除
}

int main() {
    // 1. 创建/获取系统共享内存段
    int shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        return 1;
    }

    // 2. 附加共享内存到进程地址空间
    shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (void*)-1) {
        perror("shmat failed");
        return 1;
    }

    // 3. 初始化文件系统（仅首次创建时）
    static int is_first_process = 0;
    if (shared_memory == (void*)-1) {
        perror("shmat failed");
        return 1;
    }
    
    // 检查是否为首次初始化
    if (*(int*)shared_memory == 0) {
        is_first_process = 1;
        fs = (fSystem*)shared_memory;
        memset(fs, 0, sizeof(fSystem));
        fs->path_count = 0;
        
        // 初始化根目录
        root_dir = &fs->inodes[0];
        root_dir->inode_tag = 1;
        root_dir->type = 1;
        root_dir->myIndex = 0;
        root_dir->parent_index = 0;  // 根目录的父目录是自身
        strcpy(root_dir->path, "/");
        strcpy(root_dir->name, "/");
        root_dir->son_namelist = NULL;
        root_dir->son_index_list = NULL;
        root_dir->list_lastpoint = -1;
        
        // 初始化根目录信号量
        init_inode_sem(root_dir);
        
        // 标记共享内存已初始化
        *(int*)shared_memory = 1;
        
        printf("File system initialized in shared memory\n");
    } else {
        fs = (fSystem*)shared_memory;
        printf("Connected to existing file system in shared memory\n");
    }

    // 交互式模式
    char input[256];
    char cmd[64], arg1[128], arg2[128], arg3[128];
    
    while (1) {
        printf("%s$ ", get_current_path());
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nExiting...\n");
            break;
        }
        
        // 移除换行符
        input[strcspn(input, "\n")] = '\0';
        
        // 解析命令
        int args = sscanf(input, "%s %s %s %s", cmd, arg1, arg2, arg3);
        
        if (strcmp(cmd, "mkdir") == 0 && args >= 2) {
            mkdir(arg1);
        } else if (strcmp(cmd, "rmdir") == 0 && args >= 2) {
            rmdir(arg1);
        } else if (strcmp(cmd, "open") == 0 && args >= 2) {
            open_file(arg1);
        } else if (strcmp(cmd, "rm") == 0 && args >= 2) {
            rm(arg1);
        } else if (strcmp(cmd, "cd") == 0 && args >= 2) {
            cd(arg1);
        } else if (strcmp(cmd, "ls") == 0) {
            if (args >= 2) {
                ls(arg1);
            } else {
                ls(NULL);
            }
        } else if (strcmp(cmd, "write") == 0 && args >= 3) {
            int append = 0;
            const char* path = arg1;
            const char* content = arg2;
            
            // 检查是否有追加选项
            if (args >= 4 && strcmp(arg3, "-a") == 0) {
                append = 1;
            }
            
            write_file(path, content, strlen(content), append);
        } else if (strcmp(cmd, "read") == 0 && args >= 2) {
            read_file(arg1);
        } else if (strcmp(cmd, "rename") == 0 && args >= 3) {
            my_rename(arg1, arg2);
        } else if (strcmp(cmd, "pwd") == 0) {
            pwd();
        } else if (strcmp(cmd, "exit") == 0) {
            break;
        } else {
            printf("Usage:\n"
                   "  mkdir <path> - Create directory\n"
                   "  rmdir <path> - Remove directory\n"
                   "  open <path> - Create file\n"
                   "  rm <path> - Remove file or directory\n"
                   "  cd <path> - Change directory\n"
                   "  ls [<path>] - List directory contents\n"
                   "  write <path> <content> [-a] - Write to file (use -a to append)\n"
                   "  read <path> - Read and display file content\n"
                   "  rename <old_path> <new_path> - Rename file or directory\n"
                   "  pwd - Print current working directory\n"
                   "  exit - Exit the file system\n");
        }
    }
    
    // 清理资源
    cleanup_shared_memory();
    
    // 仅在最后一个进程退出时删除共享内存（实际实现需要引用计数）
    // 这里不删除，避免意外删除
    // if (is_first_process) {
    //     shmctl(shmid, IPC_RMID, NULL);
    // }
    
    return 0;
}
