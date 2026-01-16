#include <iostream>
#include <thread>
#include <mutex>

int g_num = 0;
std::mutex myMutex1;
std::mutex myMutex2;

void test()
{
    for (unsigned i = 0; i < 1000000; ++i) {
        // 使用 std::lock 同时获取多个mutex，避免死锁
        std::lock(myMutex1, myMutex2);
        
        // 使用 std::adopt_lock 告诉 lock_guard 这些mutex已经被锁定了
        // lock_guard 只需要负责解锁，不会再次尝试加锁
        std::lock_guard<std::mutex> lg1(myMutex1, std::adopt_lock);
        std::lock_guard<std::mutex> lg2(myMutex2, std::adopt_lock);
        
        // 临界区：操作共享变量
        ++g_num;
    }
}

int main()
{
    std::thread myThread(test);
    
    for (unsigned i = 0; i < 1000000; ++i) {
        // 主线程也使用相同的模式
        std::lock(myMutex1, myMutex2);
        std::lock_guard<std::mutex> lg2(myMutex2, std::adopt_lock);
        std::lock_guard<std::mutex> lg1(myMutex1, std::adopt_lock);
        ++g_num;
    }
    
    myThread.join();
    
    std::cout << "Final value of g_num: " << g_num << std::endl;
    std::cout << "Expected value: 2000000" << std::endl;
    
    return 0;
}