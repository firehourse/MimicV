#include "queue.h"

Queue* create_queue(int capacity) {
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    if (queue == NULL) {
        return NULL;
    }
    queue->data = (int*)malloc(capacity * sizeof(int));
    if (queue->data == NULL) {
        free(queue);
        return NULL;
    }
    queue->capacity = capacity;
    queue->size = 0;
    queue->head = 0;
    queue->tail = 0;
    return queue;
}

bool enqueue(Queue *queue, int value) {
    if (queue->size == queue->capacity) {
        return false;
    }
    queue->data[queue->tail] = value;
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->size++;
    return true;
}

int dequeue(Queue *queue) {
    if (queue->size == 0) {
        return -1;
    }
    int value = queue->data[queue->head];
    queue->head = (queue->head + 1) % queue->capacity;
    queue->size--;
    return value;
}