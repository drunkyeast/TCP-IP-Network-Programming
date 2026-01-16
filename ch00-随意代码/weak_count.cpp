#include <iostream>
#include <memory>

int main() {
    auto sp = std::make_shared<int>(10);
    std::weak_ptr<int> wp1 = sp;
    std::weak_ptr<int> wp2 = sp;
    std::weak_ptr<int> wp3 = sp;

    // 此时：强引用=1， 弱引用=3

    // 看看 weak_ptr.use_count() 返回啥？
    std::cout << "wp1.use_count() = " << wp1.use_count() << std::endl; 
    
    // 增加一个强引用
    auto sp2 = sp;
    std::cout << "增加强引用后，wp1.use_count() = " << wp1.use_count() << std::endl;

    return 0;
}