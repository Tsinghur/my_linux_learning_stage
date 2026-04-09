#include <my_header.h>
#include "thread_pool.h"
#include "socket.h"
#include "epoll.h"

int pipe_fd[2];

void func(int num){
    printf("signal num = %d\n", num);
    write(pipe_fd[1], "1", 1);
}

int main(int argc, char* argv[]){
    pipe(pipe_fd);

    if(fork() != 0){ // 父进程
        signal(2, func);
        wait(NULL);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        exit(0);
    }
    // 子进程
    setpgid(0, 0);
    close(pipe_fd[1]);

    thread_pool_t pool;
    memset(&pool, 0, sizeof(pool));
    init_thread_pool(&pool, 4);

    int server_fd;
    init_socket("192.168.85.128", "12345", &server_fd);

    int epoll_fd = epoll_create(1);
    ERROR_CHECK(epoll_fd, -1, "epoll_create");

    epoll_add_fd(epoll_fd, server_fd);
    epoll_add_fd(epoll_fd, pipe_fd[0]);

    while(1){
        struct epoll_event events[10];
        int ready_count = epoll_wait(epoll_fd, events, 10, -1);
        printf("ready count = %d\n", ready_count);
        ERROR_CHECK(ready_count, -1, "epoll_wait");

        for(int i = 0; i < ready_count; i++){
            int fd = events[i].data.fd;

            if(fd == pipe_fd[0]){
                char buf;
                read(fd, &buf, sizeof(buf));
                printf("子进程收到了父进程的结束信号\n");

                /* pthread_mutex_lock(&pool.mutex); */
                pool.exit_flag = true;
                pthread_cond_broadcast(&pool.cond);
                /* pthread_mutex_unlock(&pool.mutex); */

                for(int i = 0; i < pool.thread_num; i++)
                    pthread_join(pool.thread_id_arr[i], NULL);
                pthread_exit(NULL);
            }
            else if(fd == server_fd){
                int client_fd = accept(fd, NULL, NULL);
                pthread_mutex_lock(&pool.mutex);
                enQueue(&pool.Queue, client_fd);
                pthread_cond_signal(&pool.cond);
                pthread_mutex_unlock(&pool.mutex);
            }
        }
    }

    return 0;
}
