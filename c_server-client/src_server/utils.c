#include "server.h"

static int set_fd_nonblock(int fd);

int epoll_add(int fd_epoll, int fd, uint32_t flag, bool is_noblock) {
    epoll_event ev = {0};
    int         ret;

    ev.events = EPOLLIN;
    if (flag == EPOLLET || flag == EPOLLONESHOT) {
        ev.events = EPOLLIN | flag;
    }
    ev.data.fd = fd;

    ret = epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd, &ev);
    if (ret == -1) {
        perror("E! Server: epoll_ctl()");
        return -1;
    }

    if (is_noblock == true) {
        ret = set_fd_nonblock(fd);
    }

    return ret;
}

void epoll_del(int fd_epoll, int fd) {
    if (epoll_ctl(fd_epoll, EPOLL_CTL_DEL, fd, NULL) == -1) {
        perror("I! Server: failed to delete file descriptor, epoll_ctl():");
    }
    return;
}

static int set_fd_nonblock(int fd) {
    int flags;

    flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("E! Server: fcntl()");
        return -1;
    }

    flags |= O_NONBLOCK;
    flags = fcntl(fd, F_SETFL, flags);

    if (flags == -1) {
        perror("E! Server: fcntl()");
        return -1;
    }

    return 0;
}

void conn_print_accepted_info(sockaddr_storage *addr_connected) {
    char        addr_netw[INET6_ADDRSTRLEN];
    const void *ret;

    if (!addr_connected) {
        return;
    }

    ret = inet_ntop(addr_connected->ss_family, conn_get_in_addr((sockaddr *)addr_connected),
                    addr_netw, sizeof(addr_netw));
    if (ret == NULL) {
        perror("E! Server: inet_ntop()");
        return;
    }
}

// get sockaddr, IPv4 or IPv6:
void *conn_get_in_addr(sockaddr *sa) {
    if (!sa) {
        return NULL;
    }
    if (sa->sa_family == AF_INET) {
        return &(((sockaddr_in *)sa)->sin_addr);
    }
    return &(((sockaddr_in6 *)sa)->sin6_addr);
}