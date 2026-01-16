#include <iostream>
#include <memory>

int main() {
    auto p = std::make_unique<int>(42);
    std::cout << *p << std::endl;
    auto p2 = std::make_shared<int>(10);
    std::cout << *p2 << std::endl;
    return 0;
}