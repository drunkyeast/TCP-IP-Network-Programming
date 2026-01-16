#include <iostream>
#include <thread>
#include <unistd.h>
using namespace std;

int main() {
    cout << "main thread id: " << this_thread::get_id() << endl;
    thread t1([]() {
        cout << "thread 1 id: " << this_thread::get_id() << endl;
        sleep(10);
    });
    t1.join();
    return 0;
}