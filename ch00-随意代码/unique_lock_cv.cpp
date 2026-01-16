#include <mutex>
#include <condition_variable>
#include <queue>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> data_queue;
bool finished = false;

// 消费者线程
void consumer() {
    // 1. 必须使用 unique_lock
    std::unique_lock<std::mutex> lock(mtx);
    
    // 2. 使用 while 循环检查条件 (防止虚假唤醒，下面会讲)
    // 意思是：只要队列是空的，且没结束，我就一直等
    while (data_queue.empty() && !finished) {
        // 3. wait 接受 lock
        // 内部流程：unlock -> sleep -> (收到 notify) -> lock -> return
        cv.wait(lock); 
    }

    if (!data_queue.empty()) {
        int data = data_queue.front();
        data_queue.pop();
        // 处理数据...
    }
}

// 生产者线程
void producer() {
    // 这里的锁可以用 lock_guard，因为生产者不需要中途解锁
    // 但为了统一习惯，用 unique_lock 也可以
    {
        std::lock_guard<std::mutex> lock(mtx);
        data_queue.push(42);
    } // 出了作用域自动解锁
    
    // 通知等待的线程（通知时不一定要持有锁，但通常建议出了临界区再通知）
    cv.notify_one(); 
}