#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <my_header.h>
#include "queue.h"
#include "worker.h"

typedef struct thread_pool_t{
    int thread_num;
    pthread_t* thread_id_arr;
    Queue Queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool exit_flag;
}thread_pool_t;

void init_thread_pool(thread_pool_t* pool, int num);

#endif
