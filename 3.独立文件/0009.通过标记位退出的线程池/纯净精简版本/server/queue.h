#ifndef QUEUE_H
#define QUEUE_H

typedef struct node_s{
    int fd;
    struct node_s* next;
}Node;

typedef struct queue_s{
    Node* front;
    Node* rear;
    int size;
}Queue;

// 入队
void enQueue(Queue* Queue, int fd);

// 出队
int deQueue(Queue* Queue);

#endif
