#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define REPEAT_TIMES 10000
#define BILLION  1000000000L

int main() {
    int stride;
    int size = 1024 * 1024; // �����С��Լ4MB��1024*1024��int��ÿ��intռ4�ֽڣ�
    int *array = (int*)malloc(size * sizeof(int));
    struct timespec start, end;

    // ��ʼ������
    for (int i = 0; i < size; i++) {
        array[i] = 1;
    }

    for (stride = 1; stride <= 64; stride++) {
        volatile int sum = 0; // ʹ��volatile��ֹ�������Ż�
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (int repeat = 0; repeat < REPEAT_TIMES; repeat++) {
            for (int i = 0; i < size; i += stride) {
                sum += array[i];
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        long seconds = end.tv_sec - start.tv_sec;
        long nanoseconds = end.tv_nsec - start.tv_nsec;
        double elapsed = seconds + nanoseconds*1e-9;
        printf("Stride %d, Time taken: %.9f seconds\n", stride, elapsed / REPEAT_TIMES);
    }

    free(array);
    return 0;
}
    
