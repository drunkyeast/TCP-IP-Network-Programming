#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for sleep
#include <pthread.h>

void* thread_func(void* arg) {
    printf("Hello, World! 1\n");
    sleep(1); // 睡1秒
    printf("Hello, World! 2\n");
    sleep(1); // 睡1秒
    printf("Hello, World! 3\n");
    // return NULL;
    exit(0);
}

int main() {
    pthread_t t1;
    // 创建线程
    if (pthread_create(&t1, NULL, thread_func, NULL) != 0) {
        perror("Thread creation failed");
        return 1;
    }

    // 注意：这里没有 pthread_join，也没有 detach
    // 主线程直接结束
    sleep(5);
    printf("Main thread is returning...\n");
    return 0; 
}