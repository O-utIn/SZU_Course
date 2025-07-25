#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BLOCK_SIZE 4096

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
} Inode;

typedef struct fSystem {
    Inode inodes[200];    // inode 表，共 200 个
    int d_bmap[25344];    // 数据块位图，共 25344 个数据块
    Data_block data_region[25344]; // 数据块区域
} fSystem;

void* shared_memory = NULL;
fSystem* fs = NULL;  // 指向文件系统的指针
Inode* root_dir = NULL;
char current_path[256] = "/";  // 当前工作目录

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
        if (strcmp(current_path, "/") == 0) {
            sprintf(abs_path, "/%s", path);
        } else {
            sprintf(abs_path, "%s/%s", current_path, path);
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
        if (strcmp(current_path, "/") == 0) {
            sprintf(temp, "/%s", path);
        } else {
            sprintf(temp, "%s/%s", current_path, path);
        }
        normalize_path(temp, resolved);
    }
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
    if (strcmp(resolved, current_path) == 0) {
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
        if (strcmp(resolved, current_path) == 0) {
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
    strcpy(current_path, resolved);
    return 0;
}

// 列出目录内容
void ls(const char* path) {
    // 解析路径
    char resolved[256];
    if (path == NULL || strlen(path) == 0) {
        strcpy(resolved, current_path);
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
    
    // 输出目录内容
    printf("Contents of directory '%s':\n", resolved);
    for (int i = 0; i <= dir->list_lastpoint; i++) {
        if (dir->son_namelist[i] != NULL) {
            printf("  %s\n", dir->son_namelist[i]);
        }
    }
}

// 写入文件（支持覆盖和追加模式）
int write_file(const char* path, const char* content, int size, int append) {
    // 解析路径
    char resolved[256];
    resolve_path(path, resolved);
    
    // 查找文件
    Inode* file = find_inode(resolved);
    if (file == NULL) {
        printf("Error: File '%s' not found\n", resolved);
        return -1;
    }
    
    // 确保是文件类型
    if (file->type != 0) {
        printf("Error: '%s' is not a file\n", resolved);
        return -1;
    }
    
    if (append && file->size > 0) {
        // 追加模式：保留原有内容，在末尾添加新内容
        char* buffer = (char*)malloc(file->size + size);
        if (!buffer) {
            printf("Error: Memory allocation failed\n");
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
    
    printf("File '%s' written successfully\n", resolved);
    return 0;
}

// 读取文件内容并打印
int read_file(const char* path) {
    // 解析路径
    char resolved[256];
    resolve_path(path, resolved);
    
    // 查找文件
    Inode* file = find_inode(resolved);
    if (file == NULL) {
        printf("Error: File '%s' not found\n", resolved);
        return -1;
    }
    
    // 确保是文件类型
    if (file->type != 0) {
        printf("Error: '%s' is not a file\n", resolved);
        return -1;
    }
    
    // 检查文件是否为空
    if (file->size == 0) {
        printf("File '%s' is empty\n", resolved);
        return 0;
    }
    
    // 读取并打印文件内容
    printf("Content of file '%s':\n", resolved);
    printf("----------------------------------------\n");
    
    for (int i = 0; i < file->size; i++) {
        int block_num = i / BLOCK_SIZE;
        int offset = i % BLOCK_SIZE;
        putchar(fs->data_region[file->start_data_index + block_num].data[offset]);
    }
    
    printf("\n----------------------------------------\n");
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
    printf("%s\n", current_path);
}

int main() {
    // 分配共享内存
    shared_memory = malloc(100 * 1024 * 1024);
    if (shared_memory == NULL) {
        printf("Error: Failed to allocate memory\n");
        return 1;
    }
    
    // 初始化文件系统
    fs = (fSystem*)shared_memory;
    memset(fs, 0, sizeof(fSystem));
    
    // 初始化根目录
    root_dir = &fs->inodes[0];
    root_dir->inode_tag = 1;
    root_dir->type = 1;
    root_dir->myIndex = 0;
    root_dir->parent_index = -1;
    strcpy(root_dir->path, "/");
    strcpy(root_dir->name, "/");
    root_dir->son_namelist = NULL;
    root_dir->son_index_list = NULL;
    root_dir->list_lastpoint = -1;
    
    printf("Simple File System initialized\n");
    
    char input[256];
    char cmd[64], arg1[128], arg2[128], arg3[128];
    
    while (1) {
        printf("%s$ ", current_path);
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
    
    // 释放内存
    for (int i = 0; i < 200; i++) {
        if (fs->inodes[i].inode_tag == 1 && fs->inodes[i].son_namelist != NULL) {
            for (int j = 0; j <= fs->inodes[i].list_lastpoint; j++) {
                if (fs->inodes[i].son_namelist[j] != NULL) {
                    free(fs->inodes[i].son_namelist[j]);
                }
            }
            free(fs->inodes[i].son_namelist);
            free(fs->inodes[i].son_index_list);
        }
    }
    
    free(shared_memory);
    return 0;
}
