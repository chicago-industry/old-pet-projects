#ifndef __server_h__
#define __server_h__

#include <arpa/inet.h>
#include <endian.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>

typedef struct addrinfo         addrinfo;
typedef struct epoll_event      epoll_event;
typedef struct sockaddr_storage sockaddr_storage;
typedef struct sockaddr_in      sockaddr_in;
typedef struct sockaddr_in6     sockaddr_in6;
typedef struct sockaddr         sockaddr;

#define DEF_THREADS     4    // number of threads
#define DEF_BUFFER_SIZE 1024 // buffer size for reading data from clients
#define DEF_MAX_EVENTS  100
#define DEF_BACKLOG     100 // the number of pending connections will be stored in the queue

// argument for conn_handle_client()
typedef struct _conn_recv_t {
    int    fd_epoll;
    int    fd;
    size_t bytes_count;
} conn_recv_t;

int   conn_init(const char *port, int backlog);
int   epoll_init(epoll_event *ev, int fd_socket);
void *conn_create_arg(int fd_epoll, int fd_connected, size_t bytes_count);

// utils
int  epoll_add(int fd_epoll, int fd, uint32_t flag, bool is_noblock);
void epoll_del(int fd_epoll, int fd);

void *conn_get_in_addr(sockaddr *sa);
void  conn_print_accepted_info(sockaddr_storage *addr_connected);

#endif // __server_h__