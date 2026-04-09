#include <my_header.h>
#include "epoll.h"

void epoll_add_fd(int epoll_fd, int fd){
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;
    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl ADD");
}

void epoll_del_fd(int epoll_fd, int fd){
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;
    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl DEL");
}
