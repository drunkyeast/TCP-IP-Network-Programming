#include <iostream>
#include <vector>
#include <deque>

int main() {
    // --- Vector ---
    char cnt = 10;
    std::vector<int> vec(cnt);
    for (auto &v : vec) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    int a = 10;
    std::cout << decltype(a)() << std::endl;
    std::cout << int() << std::endl;
    int b = 10;
    vec.push_back(b);
    vec.emplace_back(b);
    vec.emplace_back(1,2,3);
    return 0;
}