#include <iostream>
#include <thread>
#include <mutex>
#define NUM_THREAD 10
int g_num = 0;
std::mutex g_mutex;
void *thread_dec()
{
    for (int i = 0; i < 5000000; i++) {
        g_mutex.lock(); // 注意, 这里临界区小. 太小了会导致频繁的加锁解锁.
        g_num += 1;
        g_mutex.unlock();
    }
    return NULL;
}

void *thread_inc()
{
    g_mutex.lock(); // 注意, 这里临界区很大. 太大了会阻塞其他线程.
    for (int i = 0; i < 5000000; i++)
        g_num -= 1;
    g_mutex.unlock();
    return NULL;
}

int main(int argc, char *argv[])
{
    std::thread t[NUM_THREAD];

    for (int i = 0; i < NUM_THREAD; i++)
    {
        if (i & 1)
            t[i] = std::thread(thread_dec);

        else
            t[i] = std::thread(thread_inc);
    }

    for (int i = 0; i < NUM_THREAD; i++)
        t[i].join();

    std::cout << g_num << '\n';
    return 0;
}

