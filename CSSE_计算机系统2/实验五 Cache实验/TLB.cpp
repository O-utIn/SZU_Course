#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE (10 * 1024 * 1024) // 10MB
#define PAGE_SIZE 4096 // ����ҳ���СΪ4KB

int main() {
    int *array = (int*)malloc(ARRAY_SIZE * sizeof(int)); // ��ȷ������ת��
    struct timespec start, end;

    // ��ʼ������
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i;
    }

    // ��1ҳ������ҳ
    for (int step = 1; step <= ARRAY_SIZE / PAGE_SIZE; step *= 2) {
        volatile int sum = 0;

        clock_gettime(CLOCK_MONOTONIC, &start);
        for (int i = 0; i < ARRAY_SIZE; i += step * PAGE_SIZE / sizeof(int)) { // ��������Ϊ��ҳ�����
            sum += array[i];
        }
        clock_gettime(CLOCK_MONOTONIC, &end);

        long seconds = end.tv_sec - start.tv_sec;
        long nanoseconds = end.tv_nsec - start.tv_nsec;
        double elapsed = seconds + nanoseconds*1e-9;

        printf("Step size: %d pages, Time taken: %.9f seconds\n", step, elapsed);
    }

    free(array);
    return 0;
}
    
