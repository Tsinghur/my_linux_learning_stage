#include <my_header.h>
#include "queue.h"

void enQueue(Queue* Queue, int fd){
    Node* node = (Node*)calloc(1, sizeof(Node));
    node->fd = fd;
    
    if(Queue->size == 0){
        Queue->front = node;
        Queue->rear = node;
    }else{
        Queue->front->next = node;
        Queue->rear = node;
    }
    Queue->size++;
}

int deQueue(Queue* Queue){
    if(Queue->size == 0)
        return -1;
    
    int fd = Queue->front->fd;
    Node* tmp = Queue->front;
    Queue->front = tmp->next;
    Queue->size--;

    // 若是出队后为空
    if(Queue->size == 0)
        Queue->rear = NULL; // front指针已自动置空

    free(tmp);
    return fd;
}
