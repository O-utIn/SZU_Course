#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>

#define thread_num 16
#define MB 1024 * 1024
int *array;
int length; //array length
int count;
int t; //number of thread
void *count3s_thread(void* id);

pthread_mutex_t m;  //增加一个互斥量

int main()
{
    pthread_mutex_init(&m, NULL);  //初始化互斥量
    int i;
    int tid[thread_num];
    pthread_t threads[thread_num];
    length = 64 * MB;
    array = malloc(length*4);                        //256MB
    for (i = 0; i < length; i++)                     //initial array
        array[i] = i % 2? 4 : 3;                    //偶数i对应数值3

    count = 0; // 将count初始化移到这里
    for (t = 0; t < thread_num; t++)                 //循环创建16个线程
    {
        tid[t]=t;
        int err = pthread_create(&(threads[t]), NULL, count3s_thread,&(tid[t]) );
        if (err)
        {
            printf("create thread error!\n");
            return 0;
        }
    }

    for (t = 1; t < thread_num; t++)
        pthread_join(threads[t], NULL);  //等待前面创建的计数线程结束

    printf("Total count= %d \n",count);
    pthread_mutex_destroy(&m);
    return 0;
}

void *count3s_thread(void* id)
{
     /*compute portion of the array that this thread should work on*/
     int length_per_thread = length / thread_num; //length of every thread
     int start = *(int *)id * length_per_thread; //every thread start position
     int i;

    for (i = start; i < start + length_per_thread; i++)
    {
        if (array[i] == 3)
        {
            pthread_mutex_lock(&m);  //进入临界区
            count++;
            pthread_mutex_unlock(&m);  //推出临界区
        }
    }
    return NULL; // 添加返回值语句
}
