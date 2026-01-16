#include <iostream>
#include <thread>
using namespace std;

class A {
public:
    A() {
        cout << "A() thread id: " << this_thread::get_id() << endl;
    }
    A(A& other) {
        cout << "A(A& other) thread id: " << this_thread::get_id() << endl;
    }
    A(const A& other) {
        cout << "A(const A& other) thread id: " << this_thread::get_id() << endl;
    }
    A(A&& other) {
        cout << "A(A&& other) thread id: " << this_thread::get_id() << endl;
    }
    A(const A&& other) {
        cout << "const A(A&& other) thread id: " << this_thread::get_id() << endl;
    }
};
// void func(A&& a) {
//     cout << "void func(A&& a) thread id: " << this_thread::get_id() << endl;
// }
void func(const A&& a) {
    cout << "void func(const A& a) thread id: " << this_thread::get_id() << endl;
}
int main() {
    A a;
    thread t1(func, a);
    t1.join();
    return 0;
}