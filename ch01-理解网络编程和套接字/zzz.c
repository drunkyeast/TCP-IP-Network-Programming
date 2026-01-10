#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
int socket(int domain, int type, int protocol);
// Create a new socket of type TYPE in domain DOMAIN, using
// protocol PROTOCOL. If PROTOCOL is zero, one is chosen automatically.
// Returns a file descriptor for the new socket, or -1 for errors.

int main(int argc, char* argv[])
{
    int sock;
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        printf("socket() error\n");

        #include <sys/socket.h>
    int bind(int __fd, const struct sockaddr *__addr, socklen_t __len);
    size_t a;
    return 0;
}