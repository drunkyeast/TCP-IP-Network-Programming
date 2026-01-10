#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

void* handle_clnt(void* arg);
void send_msg(char *msg, int len);
void error_handling(char* message);

pthread_mutex_t mutex;
int clnt_socks[MAX_CLNT];
int clnt_cnt = 0;

int main(int argc, char* argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_add_sz;
    pthread_t t_id;

    if(argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    pthread_mutex_init(&mutex, NULL);
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
    
    while(1)
    {
        clnt_add_sz = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_add_sz);
        if(clnt_sock == -1)
            error_handling("accept() error");
        
        pthread_mutex_lock(&mutex); // 我此时正在增加新的连接套接字. 如果另一个线程要发给所有客户端, 要遍历所有套接字, 怎么办?
        clnt_socks[clnt_cnt++] = clnt_sock; //  如果另一个线程要因为某客户端断开连接, 要删除对应的连接套接字, 怎么办? 所以有临界区.
        pthread_mutex_unlock(&mutex);

        pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
        pthread_detach(t_id); // 这里用了detach!!
        printf("Connected client IP: %s\n", inet_ntoa(clnt_addr.sin_addr));
    }
    close(serv_sock);

    return 0;
}

void* handle_clnt(void* arg)
{
    int clnt_sock = *((int*)arg);
    int str_len = 0;
    char msg[BUF_SIZE];

    while((str_len = read(clnt_sock, msg, sizeof(msg))) != 0) 
    {
        send_msg(msg, str_len); // 只要收到一个客户端信息, 就发送给所有客户端. 
        // 这里会一直循环下去, 直到客户端关闭连接. 也就是read返回0.
    }
    
    pthread_mutex_lock(&mutex); // 这是客户端关闭连接后, 要对clnt_sock进行删除. 
    for(int i = 0; i < clnt_cnt; i++) // 但同时其他客户端可能正在发送消息, 所以有临界区.
    {
        if(clnt_socks[i] == clnt_sock)
        {
            while(i++ < clnt_cnt - 1)
                clnt_socks[i] = clnt_socks[i + 1];
            break;
        }
    }
    clnt_cnt--;
    pthread_mutex_unlock(&mutex);
    close(clnt_sock);

    return NULL;
}

void send_msg(char* msg, int len)
{
    pthread_mutex_lock(&mutex); // 这里要发给所有客户端. 如果此时某个客户端关闭了怎么办? 
    for(int i = 0; i < clnt_cnt; i++) // 见handle_clnt函数, 服务器此时要删除对应的连接套接字. 所以有临界区.
    {
        write(clnt_socks[i], msg, len);
    }
    pthread_mutex_unlock(&mutex);
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}