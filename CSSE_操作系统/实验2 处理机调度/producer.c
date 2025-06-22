#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h> 
#include "shm_com_sem.h"

int main(void) {
    void *shared_memory = (void *)0;
    struct shared_mem_st *shared_stuff;
    char key_line[LINE_SIZE];
    int shmid;
    sem_t *sem_queue_mutex, *sem_queue_empty, *sem_queue_full;

    // 获取共享内存
    shmid = shmget((key_t)1234, sizeof(struct shared_mem_st), 0666 | IPC_CREAT);
    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }
    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }
    shared_stuff = (struct shared_mem_st *)shared_memory;

    // 创建信号量
    sem_queue_mutex = sem_open(queue_mutex, O_CREAT, 0666, 1);
    sem_queue_empty = sem_open(queue_empty, O_CREAT, 0666, NUM_LINE);
    sem_queue_full = sem_open(queue_full, O_CREAT, 0666, 0);
    if (sem_queue_mutex == SEM_FAILED || sem_queue_empty == SEM_FAILED || sem_queue_full == SEM_FAILED) {
        fprintf(stderr, "sem_open failed\n");
        exit(EXIT_FAILURE);
    }

    // 初始化指针
    shared_stuff->line_write = 0;
    shared_stuff->line_read = 0;

	while(1) {
	    printf("Enter your text (\"quit\" for exit): ");
	    fgets(key_line, LINE_SIZE, stdin);
	    key_line[strcspn(key_line, "\n")] = '\0'; // 处理输入

	    sem_wait(sem_queue_empty); // 等待空缓冲区
	    sem_wait(sem_queue_mutex); // 互斥锁

	    strcpy(shared_stuff->buffer[shared_stuff->line_write], key_line);
	    // 先写入，再判断是否退出
	    if (strcmp(key_line, "quit") == 0) {
		sem_post(sem_queue_mutex); // 释放互斥锁
		sem_post(sem_queue_full); // 满缓冲区加一
		break;
	    }
	    shared_stuff->line_write = (shared_stuff->line_write + 1) % NUM_LINE;

	    sem_post(sem_queue_mutex); // 释放互斥锁
	    sem_post(sem_queue_full); // 满缓冲区加一
	}

    // 释放信号量
    sem_close(sem_queue_mutex);
    sem_close(sem_queue_empty);
    sem_close(sem_queue_full);

    // 分离共享内存
    if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    // 删除共享内存
    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
