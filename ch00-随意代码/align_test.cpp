#include <iostream>
#include <cstddef> // for offsetof

// 定义一个典型的结构体
struct TestStruct {
    char a;      // 1 byte
    // 这里的填充(padding)取决于下一行的对齐要求
    void* ptr;   // 32位是4byte，64位是8byte
    int b;       // 4 byte
};

int main() {
    using namespace std;

    cout << "=== 环境检测 ===" << endl;
    cout << "指针大小(sizeof(void*)): " << sizeof(void*) << " bytes" << endl;
    
    cout << "\n=== 结构体分析 (TestStruct) ===" << endl;
    cout << "char a   大小: " << sizeof(char) << endl;
    cout << "void* ptr 大小: " << sizeof(void*) << endl;
    cout << "int b    大小: " << sizeof(int) << endl;

    cout << "\n=== 内存布局 (Offset) ===" << endl;
    cout << "char a   偏移量: " << offsetof(TestStruct, a) << endl;
    cout << "void* ptr 偏移量: " << offsetof(TestStruct, ptr) << endl;
    cout << "int b    偏移量: " << offsetof(TestStruct, b) << endl;

    cout << "\n=== 最终结果 ===" << endl;
    cout << "结构体总大小(sizeof): " << sizeof(TestStruct) << endl;
    cout << "结构体对齐要求(alignof): " << alignof(TestStruct) << endl;

    return 0;
}