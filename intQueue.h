//
// Created by ivand on 24.8.2021..
//

#ifndef SLIMEMOULDINC_QUEUE_H
#define SLIMEMOULDINC_QUEUE_H

#endif //SLIMEMOULDINC_QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

struct intQueue{
    int32_t size;
    int32_t numberOfElements;
    int32_t rear;
    int32_t front;
    int32_t* queue;
};

struct data{
    int32_t data;
    bool success;
};

struct intQueue* createIntQueue(){
    struct intQueue* q = malloc(sizeof(struct intQueue));
    q->size = 2;
    q->numberOfElements = 0;
    q->rear = -1;
    q->front = 0;
    q->queue = malloc(sizeof(int32_t) * 2);
    return q;
}

void printQueueStatus(struct intQueue* queue){
    fprintf(stdout, "size> %d, numberOfElements> %d, rear> %d, front> %d\n", queue->size, queue->numberOfElements, queue->rear, queue->front);
}

void printQueue(struct intQueue* queue){
    for (int32_t i = queue->front, j = 0; j < queue->numberOfElements; i++, j++) {
        fprintf(stdout, "%d ", queue->queue[i % queue->size]);
    }
    fprintf(stdout, "\n");
}

void enlargeIntQueue(struct intQueue* oldQueue){
    int32_t* newQueue = malloc(sizeof(int32_t) * oldQueue->size * 2);
    for (int32_t i = oldQueue->front, j = 0 ; i < oldQueue->size + oldQueue->front; ++i, j++) {
        newQueue[j] = oldQueue->queue[i % oldQueue->size];
    }
    oldQueue->front = 0;
    oldQueue->rear = oldQueue->size - 1;
    oldQueue->size *= 2;
    free((void*)oldQueue->queue);
    oldQueue->queue = newQueue;
}

bool isIntQueueFull(struct intQueue* queue){
    return queue->numberOfElements == queue->size;
}

bool isIntQueueEmpty(struct intQueue* queue){
    return queue->numberOfElements == 0;
}

void enqueueInt(struct intQueue* queue, int data){
    if(isIntQueueFull(queue)){
        enlargeIntQueue(queue);
    }

    if(queue->rear == queue->size - 1){
        queue->rear = -1;
    }

    queue->rear++;
    queue->queue[queue->rear] = data;
    queue->numberOfElements++;
}

struct data dequeueInt(struct intQueue* queue){
    struct data data;
    data.data = 0;
    data.success = false;

    if(isIntQueueEmpty(queue)){
        return data;
    }

    data.data = queue->queue[queue->front];
    data.success = true;
    queue->front++;

    if(queue->front == queue->size){
        queue->front = 0;
    }

    queue->numberOfElements--;

    return data;
}

void destroyIntQueue(struct intQueue* queue){
    free((void*)queue->queue);
    free((void*)queue);
}