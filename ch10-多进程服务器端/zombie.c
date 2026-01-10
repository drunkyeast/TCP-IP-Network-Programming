#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
    pid_t pid = fork();

    if(pid == 0)
    {
        puts("Hi! I'm a child process\n");
    }
    else
    {
        for (int i = 0; i < 30; i+=3) {
            printf("time: %d\n", i);
            sleep(3);
        }
    }

    if(pid == 0)
        puts("End child process");
    else
        puts("End parent process");
        
    return 0;
}