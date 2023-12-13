#include "server.h"

int conn_init(const char *port, int backlog) {
    addrinfo  info_hints = {0};
    addrinfo *info_server, *info_tmp;
    int       fd_socket, ret;

    info_hints.ai_family = AF_UNSPEC;     // IPv4 or IPv6
    info_hints.ai_socktype = SOCK_STREAM; // TCP
    info_hints.ai_flags = AI_PASSIVE;     // use my IP

    ret = getaddrinfo(NULL, port, &info_hints, &info_server);
    if (ret != 0) {
        dprintf(STDERR_FILENO, "E! Server: getaddrinfo(): %s\n", gai_strerror(ret));
        return -1;
    }

    // bind to the first one we can
    for (info_tmp = info_server; info_tmp != NULL; info_tmp = info_tmp->ai_next) {
        fd_socket = socket(info_tmp->ai_family, info_tmp->ai_socktype, info_tmp->ai_protocol);
        if (fd_socket == -1) {
            perror("E! Server: socket()");
            continue;
        }

        // set SO_REUSEADDR to reuse a local address and port that was occupied by a previous
        // socket with the same address and port, but has not yet freed it in the kernel
        // socket subsystem
        ret = setsockopt(fd_socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&(int){1}, sizeof(int));
        if (ret == -1) {
            perror("E! Server: setsockopt()");
            close(fd_socket);
            freeaddrinfo(info_server);
            return -1;
        }

        ret = bind(fd_socket, info_tmp->ai_addr, info_tmp->ai_addrlen);
        if (ret == -1) {
            close(fd_socket);
            perror("E! Server: bind()");
            continue;
        }

        break;
    }

    freeaddrinfo(info_server);

    if (!info_tmp) {
        dprintf(STDERR_FILENO, "E! Server: failed to bind\n");
        close(fd_socket);
        return -1;
    }

    ret = listen(fd_socket, backlog);
    if (ret == -1) {
        perror("E! Server: listen()");
        close(fd_socket);
        return -1;
    }

    printf("I! Server: successfully binded, listening\n");
    return (fd_socket);
}

int epoll_init(epoll_event *ev, int fd_socket) {
    int fd_epoll, ret;

    if (!ev) {
        return -1;
    }

    // init epoll_event
    fd_epoll = epoll_create1(0);
    if (fd_epoll == -1) {
        perror("E! Server: epoll_create1()");
        return -1;
    }

    // monitor fd_socket
    ret = epoll_add(fd_epoll, fd_socket, EPOLLET, true);
    if (ret == -1) {
        close(fd_socket);
        close(fd_epoll);
        return -1;
    }

    // monitor stdin for the CTRL-D input to exit the program safely
    ret = epoll_add(fd_epoll, STDIN_FILENO, EPOLLET, true);
    if (ret == -1) {
        epoll_del(fd_epoll, fd_socket);
        close(fd_socket);
        close(fd_epoll);
        return -1;
    }

    return fd_epoll;
}

void *conn_create_arg(int fd_epoll, int fd_connected, size_t bytes_count) {
    conn_recv_t *recv_t;

    if (fd_epoll < 0 || fd_connected < 0 || bytes_count <= 0) {
        return NULL;
    }

    recv_t = malloc(sizeof(*recv_t));
    if (!recv_t) {
        return NULL;
    }
    recv_t->fd_epoll = fd_epoll;
    recv_t->fd = fd_connected;
    recv_t->bytes_count = bytes_count;
    return recv_t;
}