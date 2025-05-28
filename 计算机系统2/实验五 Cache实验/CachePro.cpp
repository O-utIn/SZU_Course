#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    float *a, *b, *c, *b_transposed, temp;
    long int i, j, k, size, m;
    struct timeval time1, time2;

    if (argc < 2) {
        printf("\n\tUsage: %s <Row of square matrix>\n", argv[0]);
        exit(-1);
    } //if

    size = atoi(argv[1]);
    m = size * size;
    a = (float*)malloc(sizeof(float) * m);
    b = (float*)malloc(sizeof(float) * m);
    c = (float*)malloc(sizeof(float) * m);
    b_transposed = (float*)malloc(sizeof(float) * m);

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            a[i * size + j] = (float)(rand() % 1000 / 100.0);
            b[i * size + j] = (float)(rand() % 1000 / 100.0);
        }
    }

    // Transpose matrix b to b_transposed
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            b_transposed[j * size + i] = b[i * size + j];
        }
    }

    gettimeofday(&time1, NULL);
    // Perform matrix multiplication with optimized access
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            temp = 0;
            for (k = 0; k < size; k++)
                temp += a[i * size + k] * b_transposed[j * size + k];
            c[i * size + j] = temp;
        }
    }
    gettimeofday(&time2, NULL);

    time2.tv_sec -= time1.tv_sec;
    time2.tv_usec -= time1.tv_usec;
    if (time2.tv_usec < 0L) {
        time2.tv_usec += 1000000L;
        time2.tv_sec -= 1;
    }

    printf("Optimized execution time = %ld.%06ld seconds\n", time2.tv_sec, time2.tv_usec);
    free(a);
    free(b);
    free(c);
    free(b_transposed);
    return 0;
}
    
