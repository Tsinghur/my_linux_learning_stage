#include <my_header.h>
#include "send_file.h"

void send_file(int fd){
    char* file_name;
    int file_type;
    recv(fd, &file_type, sizeof(file_type), MSG_WAITALL);
    if(file_type == 0)
        file_name = "A.txt";
    else if(file_type == 1)
        file_name = "B.txt";
    else{
        file_type = -1;
        send(fd, &file_type, sizeof(file_type), MSG_NOSIGNAL);
        close(fd);
        pthread_exit(NULL);
    }

    int filename_len = strlen(file_name);
    send(fd, &filename_len, sizeof(filename_len), MSG_NOSIGNAL);

    send(fd, file_name, filename_len, MSG_NOSIGNAL);

    int file_fd = open(file_name, O_RDONLY);
    struct stat st;
    fstat(file_fd, &st);
    off_t file_size = st.st_size;
    send(fd, &file_size, sizeof(file_size), MSG_NOSIGNAL);

    sendfile(fd, file_fd, NULL, file_size);

    close(file_fd);
    close(fd);
}
