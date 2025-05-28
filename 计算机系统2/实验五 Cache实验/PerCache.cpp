#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STEP 1024 * 1024 // 1MB步长
#define MAX_SIZE 32 * 1024 * 1024 // 测试到32MB
#define REPEAT_TIMES 50 // 增加重复测试次数以提高数据稳定性
#define BILLION 1000000000L

int main() {
    int size;
    struct timespec start, end;

    for (size = STEP; size <= MAX_SIZE; size += STEP) {
        int *array = (int*)malloc(size * sizeof(int));
        for (int i = 0; i < size; i++) {
            array[i] = i;
        }
        volatile int sum = 0;
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (int repeat = 0; repeat < REPEAT_TIMES; repeat++) {
            for (int i = 0; i < size; i++) {
                sum += array[i];
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        long seconds = end.tv_sec - start.tv_sec;
        long nanoseconds = end.tv_nsec - start.tv_nsec;
        double elapsed = seconds + nanoseconds*1e-9;
        printf("Array size: %d MB, Time taken: %.9f seconds\n", size / (1024 * 1024), elapsed / REPEAT_TIMES);
        free(array);
    }
    return 0;
}
    
