#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

// queue　結構

typedef struct {
    // 操作頭
    int *data;
    // 宣告大小
    int capacity;
    // 可用大小
    int size;
    // 頭
    int head;
    // 尾
    int tail;
} Queue;

// function methods
Queue* create_queue(int capacity);
void destroy_queue(Queue *queue);
bool enqueue(Queue *queue, int value);
int dequeue(Queue *queue);
bool is_empty(Queue *queue);
bool is_full(Queue *queue);

#endif