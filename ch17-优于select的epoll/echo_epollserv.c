#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define BUF_SIZE 100
#define EPOLL_SIZE 50
void error_handling(char *buf);

int main(int argc, char* argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;
    int str_len;
    char buf[BUF_SIZE];

    int epfd, event_cnt;
    struct epoll_event event;
    struct epoll_event* ep_events;

    if(argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");
    
    epfd = epoll_create(EPOLL_SIZE); // 历史原因, 这个参数没用, 填1就行. 内核会根据情况调整epoll例程的大小.
    ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE); // EPOLL_SIZE(50)不是表示监控的数量, 例如10000个fd, 100个动了, 但我处理50个.
    // EPOLL_SIZE和ep_events结合epoll_wait看.
    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event); // 把监听套接字加入epoll例程
    while(1)
    {
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1); // 返回值与select一样, -1错误, 0超时, >0 表示填入ep_events的个数.
        // 为简化代码, 我就不处理event_cnt = -1和0的情况了.
        // 假设监控了10000个fd, 100个动了(100有事件发生), 那就返回50个, 因为我EPOLL_SIZE(50)表示我这次可以保存的最大事件数. 剩下的, 下一次epoll_wait调用再说.
        // 假设监控了10000个fd, 20个动了, 那就返回20.
        // 这个event_cnt就"干货", 下面遍历这些"干货", 而select也会返回一模一样的fd_num, 但它不知道到底是哪些动了, 它要遍历0~1023的所有fd, 一个个问"是你动了吗?".
        for(int i = 0; i < event_cnt; i++)
        {
            if(ep_events[i].data.fd == serv_sock) // 监听套接字
            {
                clnt_addr_sz = sizeof(clnt_addr);
                clnt_sock = accept(serv_sock,
                                   (struct sockaddr *)&clnt_addr, &clnt_addr_sz);
                if(clnt_sock == -1)
                    error_handling("accept() error");

                event.events = EPOLLIN;
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event); // 把连接套接字加入epoll例程
                printf("Connected client: %d\n", clnt_sock);
            }
            else // 已连接套接字
            {
                str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
                // read返回0表示读到末尾EOF.对于TCP套接字, 就是对方调用close发送FIN包关闭连接.(对于普通文件, 那就是读完了. 对于终端, Ctrl+D相当于输入EOF)
                if(str_len == 0)  // // 一句话总结：read 返回 0 意味着数据流已经正常结束，你应该关闭对应的文件描述符并清理资源。
                {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL); // 把连接套接字从epoll例程中删除
                    close(ep_events[i].data.fd);
                    printf("Closed client: %d\n", ep_events[i].data.fd);
                }
                else // 向客户端发送数据
                {
                    write(ep_events[i].data.fd, buf, str_len);
                }
            }
        }
    }
    close(serv_sock);
    close(epfd);

    return 0;
}

void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}