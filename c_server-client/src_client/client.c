#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define DEF_MESSAGES_COUNT 100000

typedef struct addrinfo     addrinfo;
typedef struct sockaddr_in  sockaddr_in;
typedef struct sockaddr_in6 sockaddr_in6;
typedef struct sockaddr     sockaddr;

static void *get_in_addr(sockaddr *sa);
static int   conn_init(const char *ip, const char *port);

int main(int ac, char *av[]) {
    int      i, fd_socket;
    ssize_t  bytes_send;
    uint64_t num;

    if (ac != 3) {
        dprintf(STDERR_FILENO, "Usage: %s <ip/hostname> <port>\n", av[0]);
        return 1;
    }

    for (i = 0; i < DEF_MESSAGES_COUNT; i++) {
        fd_socket = conn_init(av[1], av[2]);
        if (fd_socket == -1) {
            return 1;
        }

        num = htobe64(i);
        bytes_send = send(fd_socket, &num, sizeof(num), 0);
        if (bytes_send == -1) {
            perror("E! send()");
            close(fd_socket);
            return 1;
        }
        printf("I! Client: sent [%d]\n", i);
        close(fd_socket);
    }

    printf("sended: %d messages ([0] - [%d])\n", i, i - 1);
    return 0;
}

// get sockaddr, IPv4 or IPv6:
static void *get_in_addr(sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((sockaddr_in *)sa)->sin_addr);
    }
    return &(((sockaddr_in6 *)sa)->sin6_addr);
}

static int conn_init(const char *ip, const char *port) {
    char      addr_netw[INET6_ADDRSTRLEN];
    addrinfo  info_hints = {0};
    addrinfo *info_server, *info_tmp;
    int       fd_socket, ret;

    info_hints.ai_family = AF_UNSPEC;
    info_hints.ai_socktype = SOCK_STREAM;

    ret = getaddrinfo(ip, port, &info_hints, &info_server);
    if (ret != 0) {
        dprintf(STDERR_FILENO, "E! Client: getaddrinfo(): %s\n", gai_strerror(ret));
        return -1;
    }

    // bind to the first one we can
    for (info_tmp = info_server; info_tmp != NULL; info_tmp = info_tmp->ai_next) {
        if ((fd_socket =
                 socket(info_tmp->ai_family, info_tmp->ai_socktype, info_tmp->ai_protocol)) == -1) {
            perror("E! Client: socket()");
            continue;
        }

        if (connect(fd_socket, info_tmp->ai_addr, info_tmp->ai_addrlen) == -1) {
            close(fd_socket);
            perror("E! Client: connect()");
            continue;
        }

        break;
    }

    if (!info_tmp) {
        dprintf(STDERR_FILENO, "E! Client: failed to connect\n");
        return -1;
    }

    if (inet_ntop(info_tmp->ai_family, get_in_addr((sockaddr *)info_tmp->ai_addr), addr_netw,
                  sizeof(addr_netw)) != NULL) {
        printf("I! Client: connecting to %s\n", addr_netw);
    }

    freeaddrinfo(info_server);
    return fd_socket;
}