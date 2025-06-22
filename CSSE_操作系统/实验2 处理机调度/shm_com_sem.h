

#define LINE_SIZE 256
#define NUM_LINE 16

// 用于创建信号量时的识别id
const char *queue_mutex = "queue_mutex";
const char *queue_empty = "queue_empty";
const char *queue_full = "queue_full";

// 生产者消费者公用的缓冲区，含读写指针
struct shared_mem_st {
    char buffer[NUM_LINE][LINE_SIZE];
    int line_write;
    int line_read;
};
