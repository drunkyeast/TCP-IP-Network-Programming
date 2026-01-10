#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

int main(int argc, char* argv[])
{
    fd_set reads, temps; // fd_set是1024 bits, 1024 / 8 = 128 bytes
    // fd_set 本质上是一个 1024 位的位图, 只能监控前1024个文件描述符, 这也是select的硬伤.
    int result, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;

    FD_ZERO(&reads); // 清空名单
    FD_SET(0, &reads); // 把文件描述符0(stdin)加入监控名单, 他妈的是这样加入的啊.

    while(1)
    {
        temps = reads;
        timeout.tv_sec = 5; // 5秒
        timeout.tv_usec = 0; // 0微秒
        result = select(1, &temps, 0, 0, &timeout); 
        // 这里的1的含义是:"请内核检查从 0 开始，到第 1 个位为止（不含 1）". 而文件描述0恰好是stdin
        if(result == -1)
        {
            puts("select() error!");
            break;
        }
        else if(result == 0)
        {
            puts("Time-out!");
        }
        else
        {
            if(FD_ISSET(0, &temps)) // 检查文件描述符 fd 是否在集合 set 中被标记为“就绪”状态。
            {
                str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = 0;
                printf("Message from console: %s\n", buf);
            }
        }
    }

    return 0;
}