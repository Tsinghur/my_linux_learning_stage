#ifndef EPOLL_H
#define EPOLL_H

#include "thread_pool.h"

void epoll_add_fd(int epoll_fd, int fd);

void epoll_del_fd(int epoll_fd, int fd);

#endif
