#include <iostream>
#include <future>
int add(int a, int b) {
 return a + b;
}
int main() {
 // 使用 std::async 启动线程并返回 std::future
 std::future<int> result = std::async(std::launch::async, add, 5, 3);
 // 获取线程结果
 std::cout << "Result: " << result.get() << std::endl; // 相当于join, 这个会阻塞主线程, 直到获取工作线程返回值.
 return 0;
}
