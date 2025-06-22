#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h> 
#include "shm_com_sem.h"

int main() {
    void *shared_memory = (void *)0;
    struct shared_mem_st *shared_stuff;
    int shmid;
    pid_t fork_result;
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

    // 获取信号量
    sem_queue_mutex = sem_open(queue_mutex, 0);
    sem_queue_empty = sem_open(queue_empty, 0);
    sem_queue_full = sem_open(queue_full, 0);
    if (sem_queue_mutex == SEM_FAILED || sem_queue_empty == SEM_FAILED || sem_queue_full == SEM_FAILED) {
        fprintf(stderr, "sem_open failed\n");
        exit(EXIT_FAILURE);
    }

    fork_result = fork();
    if (fork_result == -1) {
        fprintf(stderr, "Fork failure\n");
        exit(EXIT_FAILURE);
    }

    if (fork_result == 0) { // 子进程
	while(1) {
	    sem_wait(sem_queue_full); // 等待满缓冲区
	    sem_wait(sem_queue_mutex); // 互斥锁

	    if (strcmp(shared_stuff->buffer[shared_stuff->line_read], "quit") == 0) {
		sem_post(sem_queue_mutex);
		sem_post(sem_queue_empty);
		break; // 退出子进程循环
	    }

	    printf("Child process %d reads: %s\n", getpid(), shared_stuff->buffer[shared_stuff->line_read]);
	    memset(shared_stuff->buffer[shared_stuff->line_read], 0, LINE_SIZE);
	    shared_stuff->line_read = (shared_stuff->line_read + 1) % NUM_LINE;

	    sem_post(sem_queue_mutex); // 释放互斥锁
	    sem_post(sem_queue_empty); // 空缓冲区加一

	    sleep(1); // 时延
	}
    } else { // 父进程
	while(1) {
	    sem_wait(sem_queue_full); // 等待满缓冲区
	    sem_wait(sem_queue_mutex); // 互斥锁

	    if (strcmp(shared_stuff->buffer[shared_stuff->line_read], "quit") == 0) {
		sem_post(sem_queue_mutex);
		sem_post(sem_queue_empty);
		break; // 退出父进程循环
	    }

	    printf("Parent process %d reads: %s\n", getpid(), shared_stuff->buffer[shared_stuff->line_read]);
	    memset(shared_stuff->buffer[shared_stuff->line_read], 0, LINE_SIZE);
	    shared_stuff->line_read = (shared_stuff->line_read + 1) % NUM_LINE;

	    sem_post(sem_queue_mutex); // 释放互斥锁
	    sem_post(sem_queue_empty); // 空缓冲区加一

	    sleep(1); // 时延
	}
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

    exit(EXIT_SUCCESS);
}
