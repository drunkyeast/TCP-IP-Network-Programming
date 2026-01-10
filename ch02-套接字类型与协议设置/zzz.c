#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char* argv[])
{
    int sock;
    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock == -1)
        printf("socket() error\n");
    sock = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(sock == -1)
        printf("socket() error\n");
    htonl(INADDR_ANY);
    
    return 0;
}