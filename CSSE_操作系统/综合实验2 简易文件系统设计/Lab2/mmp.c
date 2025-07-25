#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define SIZE_MB (100 * 1024 * 1024)  // 100MB

int main() {
    // 1. 获取共享内存的 key 值
    key_t key = ftok(".", 'M');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // 2. 创建共享内存段
    int shmid = shmget(key, SIZE_MB, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // 3. 连接（映射）共享内存到进程地址空间
    char *shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (char *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // 4. 等待用户输入，模拟使用共享内存
    printf("共享内存已创建，输入任意内容以继续...\n");
    char input[256];
    fgets(input, sizeof(input), stdin);

    // 5. 解除映射共享内存
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    // 6. 删除共享内存段
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }

    printf("共享内存已删除。\n");
    return 0;
}

