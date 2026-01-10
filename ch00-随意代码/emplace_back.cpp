#include <bits/stdc++.h>
using namespace std;
class  Person {
public:
    Person(string _name, int _age):name(_name), age(_age) {
        cout << "Person() " << name << " " << age << endl;
    }
    ~Person() {
        cout << "~Person() " << name << " " << age << endl;
    }
    Person(const Person& other):name(other.name), age(other.age) {
        cout << "Person(const Person& other) " << name << " " << age << endl;
    }
    Person(Person&& other):name(move(other.name)), age(other.age) {
        cout << "Person(Person&& other) " << name << " " << age << endl;
    }
    string name;
    int age;
};
int main() {
    vector<Person> vec;
    vec.push_back(Person("Tom", 11)); // 1次临时对象构造 + 1次移动构造 + 1次临时对象析构
    // vec.push_back(move(Person("Tom", 20))); // 与前面完全一样, 都是右值
    // vec.emplace_back(Person("Tom", 20)); // 前两个都是右值, 通过右值引用等价于使用这个.
    vec.pop_back();
    cout << "----------------------------------------" << endl;
    string a = "Jerry";
    vec.emplace_back(a, 22); // 仅1次普通的构造函数调用.
    vec.pop_back();
    cout << "----------------------------------------" << endl;
    auto p = Person("test3", 33);
    cout << "----------------------------------------" << endl;
    vec.push_back(p); // 底层construct函数是1次拷贝构造函数
    vec.pop_back();
    cout << "----------------------------------------" << endl;
    vec.emplace_back(p); // 底层construct函数是1次拷贝构造函数
    vec.pop_back();
    cout << "----------------------------------------" << endl;
    return 0;
}
