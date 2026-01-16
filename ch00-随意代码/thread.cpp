#include <iostream>
#include <thread>
using namespace std;

int main() {
    thread t1([]() {
        cout << "Hello, World!" << endl;
        this_thread::sleep_for(chrono::seconds(1));
        cout << "Hello, World! 2" << endl;
        this_thread::sleep_for(chrono::seconds(1));
        cout << "Hello, World! 3" << endl;
        // exit(0);
    });
    // t1.join();
    this_thread::sleep_for(5s);
    cout << "Main thread is returning..." << endl;
    return 0;
}