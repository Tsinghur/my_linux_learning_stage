#include "thread_pool.h"

void init_thread_pool(thread_pool_t* pool, int num){
    // 这个是局部指针，不应该对其本身重新赋值！！！而是应该操作其所指的空间内容
   /* pool = (thread_pool_t*)calloc(1, sizeof(thread_pool_t)); */ 
   pool->thread_num = num;
   pool->thread_id_arr = (pthread_t*)calloc(num, sizeof(pthread_t));
   pthread_mutex_init(&pool->mutex, NULL);
   pthread_cond_init(&pool->cond, NULL);

   for(int i = 0; i < num; i++){
        int ret = pthread_create(&pool->thread_id_arr[i], NULL, thread_func, pool);
        ERROR_CHECK(ret, -1, "pthread_create");
   }
}
