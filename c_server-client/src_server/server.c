#include "server.h"
#include "threadpool.h"

static void *conn_handle_client(void *__recv_t);
static int   conn_handle_socket(int fd_epoll, int fd_socket);
static bool  conn_handle_stdin(bool *stop);

int main(int ac, char *av[]) {
    int          fd_socket, fd_epoll, fd_tmp, num_events, ret, i;
    epoll_event  events[DEF_MAX_EVENTS], ev = {0};
    tp_main_t    tp_m = {0};
    conn_recv_t *recv_t;
    bool         stop = false;

    if (ac != 2 || !av[1][0]) {
        dprintf(STDERR_FILENO, "Usage: %s <port>\n", av[0]);
        return 1;
    }

    fd_socket = conn_init(av[1], DEF_BACKLOG);
    if (fd_socket == -1) {
        return 1;
    }

    fd_epoll = epoll_init(&ev, fd_socket);
    if (fd_epoll == -1) {
        return 1;
    }

    // create thread pool
    ret = tp_create(&tp_m, DEF_THREADS);
    if (ret == -1) {
        epoll_del(fd_epoll, fd_socket);
        epoll_del(fd_epoll, STDIN_FILENO);
        close(fd_socket);
        close(fd_epoll);
        return 1;
    }

    while (stop != true) {
        num_events = epoll_wait(fd_epoll, events, DEF_MAX_EVENTS, -1);
        if (num_events == -1) {
            perror("E! Server: epoll_wait()");
        } else {
            for (i = 0; i < num_events; i++) {
                fd_tmp = events[i].data.fd;

                if (fd_tmp == STDIN_FILENO) {
                    if (conn_handle_stdin(&stop)) {
                        break;
                    }
                } else if (fd_tmp == fd_socket) {
                    ret = conn_handle_socket(fd_epoll, fd_socket);
                    if (ret == -1) {
                        stop = true;
                        break;
                    }
                } else if (events[i].events & EPOLLIN) {
                    recv_t = conn_create_arg(fd_epoll, fd_tmp, sizeof(uint64_t));
                    if (!recv_t) {
                        dprintf(STDERR_FILENO, "E! Server: couldn't create an arguments!\n");
                        epoll_del(fd_epoll, fd_tmp);
                        close(fd_tmp);
                        continue;
                    }

                    ret = tp_job_add(&tp_m, conn_handle_client, (void *)recv_t);
                    if (ret == -1) {
                        dprintf(STDERR_FILENO, "E! Server: couldn't add a job!\n");
                        epoll_del(fd_epoll, fd_tmp);
                        close(fd_tmp);
                    }
                }
            }
        }
    }
    tp_wait(&tp_m);
    tp_destroy(&tp_m);
    epoll_del(fd_epoll, fd_socket);
    epoll_del(fd_epoll, STDIN_FILENO);
    close(fd_socket);
    close(fd_epoll);
    return 0;
}

static void *conn_handle_client(void *__recv_t) {
    conn_recv_t *recv_t;
    char         buffer[DEF_BUFFER_SIZE + 1];
    ssize_t      ret;
    size_t       bytes_read = 0;
    uint64_t     num_network;

    if (!__recv_t) {
        return NULL;
    }

    recv_t = (conn_recv_t *)__recv_t;

    if (recv_t->bytes_count > DEF_BUFFER_SIZE) {
        goto fail;
    }

    while (bytes_read < recv_t->bytes_count) {
        ret = recv(recv_t->fd, buffer, DEF_BUFFER_SIZE, 0);
        if (ret < 0) {
            perror("E! Server: recv()");
            goto fail;
        } else if (ret == 0) {
            break;
        } else {
            bytes_read += ret;
        }
    }

    if (bytes_read > recv_t->bytes_count || bytes_read < recv_t->bytes_count) {
        dprintf(STDERR_FILENO, "E! Server: received %s data than expected! [%ld] bytes read\n",
                bytes_read > recv_t->bytes_count ? "more" : "less", bytes_read);
        goto fail;
    }

    buffer[bytes_read] = '\0';
    memcpy(&num_network, buffer, sizeof(num_network));
    num_network = be64toh(num_network);
    printf("I! Server: got data: [%ld]\n", num_network);
    goto done;

fail:
    ret = -1;
done:
    epoll_del(recv_t->fd_epoll, recv_t->fd);
    close(recv_t->fd);
    free(recv_t);
    if (ret == -1) {
        return NULL;
    }
    return (void *)1;
}

static int conn_handle_socket(int fd_epoll, int fd_socket) {
    int              fd_connect, ret;
    sockaddr_storage addr_connected;
    socklen_t        sin_size;

    while (1) {
        sin_size = sizeof(addr_connected);
        fd_connect = accept(fd_socket, (sockaddr *)&addr_connected, &sin_size);
        if (fd_connect == -1) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                perror("E! Server: accept()");
                return -1;
            }
            break;
        }

        ret = epoll_add(fd_epoll, fd_connect, EPOLLONESHOT, false);
        if (ret == -1) {
            perror("E! Server: epoll_ctl()");
            close(fd_connect);
        }
    }
    return 0;
}

static bool conn_handle_stdin(bool *stop) {
    int ch;

    if (!stop) {
        return false;
    }

    while (1) {
        ch = getchar();
        if (ch == EOF || ch == '\n') {
            if (ch == EOF) {
                *stop = true;
            }
            break;
        }
    }
    return *stop;
}
