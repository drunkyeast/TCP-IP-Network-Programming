#include<iostream>
#include<memory>

class B;

class A
{
public:
    std::shared_ptr<B> sharedB;
};

class B
{
public:
    std::shared_ptr<A> sharedA; // 如果这里改成weak_ptr, 最终输出1, 2.
};

int main() {
    std::shared_ptr<A> sharedA = std::make_shared<A>();
    std::shared_ptr<B> sharedB = std::make_shared<B>();
    std::cout << sharedA.use_count() << std::endl; // 输出: 1
    std::cout << sharedB.use_count() << std::endl; // 输出: 1
    sharedA->sharedB = sharedB; // 让原来sharedA->sharedB指向的对象引用计数-1(但原本就是nullptr, 不会-1), sharedB引用计数+1.
    std::cout << sharedA.use_count() << std::endl; // 输出: 1
    std::cout << sharedB.use_count() << std::endl; // 输出: 2
    sharedB->sharedA = sharedA; // 一样的, p1 = p2这样的赋值, p1的引用计数会-1(原本为nullptr,就不会-1), sharedA引用计数+1.
    std::cout << sharedA.use_count() << std::endl; // 输出: 2
    std::cout << sharedB.use_count() << std::endl; // 输出: 2
    return 0;
}