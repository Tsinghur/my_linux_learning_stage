#include <my_header.h>
#include "worker.h"

void* thread_func(void* arg){
    thread_pool_t* pool = (thread_pool_t*)arg;

    while(1){
        pthread_mutex_lock(&pool->mutex);

        while(pool->Queue.size == 0 && pool->exit_flag == false)
            pthread_cond_wait(&pool->cond, &pool->mutex);

        if(pool->exit_flag == true){
            pthread_mutex_unlock(&pool->mutex);
            pthread_exit(NULL);
        }

        int fd = deQueue(&pool->Queue);

        pthread_mutex_unlock(&pool->mutex);

        send_file(fd);

        close(fd);
    }

    return NULL;
}
