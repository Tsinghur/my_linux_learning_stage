#include <my_header.h>
#include "socket.h"

void init_socket(char* ip, char* port, int* fd){
    *fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(*fd, -1, "socket");

    int opt = 1;
    setsockopt(*fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(atoi(port));

    int ret_bind = bind(*fd, (struct sockaddr*)&addr, sizeof(addr));
    ERROR_CHECK(ret_bind, -1, "bind");

    int ret_listen = listen(*fd, 100);
    ERROR_CHECK(ret_listen, -1, "listen");
}
