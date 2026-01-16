#include <iostream>
#include <semaphore.h>
#include <thread>

static sem_t sem_one;
static sem_t sem_two;
static sem_t sem_three;

int main(int argc, char* argv[])
{
    sem_init(&sem_one, 0, 1); // 信号量初始为1
    sem_init(&sem_two, 0, 0); // 信号量初始为0
    sem_init(&sem_three, 0, 0); // 信号量初始为0
    
    std::thread t1([&]() {
        for(int i = 0; i < 5; i++)
        {
            sem_wait(&sem_one);
            std::cout << "1" << std::endl;
            sem_post(&sem_two);
        }
    });
    std::thread t2([&]() {
        for(int i = 0; i < 5; i++)
        {
            sem_wait(&sem_two);
            std::cout << "2" << std::endl;
            sem_post(&sem_three);
        }
    });
    std::thread t3([&]() {
        for(int i = 0; i < 5; i++)
        {
            sem_wait(&sem_three);
            std::cout << "3" << std::endl;
            sem_post(&sem_one);
        }
    });
    t1.join();
    t2.join();
    t3.join();

    sem_destroy(&sem_one);
    sem_destroy(&sem_two);
    return 0;
}