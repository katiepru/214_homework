#ifndef _SYNCH_QUEUE_H

//Includes
#include <stdio.h>
#include <stdlib.h>

//Struct definitions
typedef struct QueueNode QueueNode;
struct QueueNode {
    QueueNode *next;
    void *data;
};

typedef struct SynchQueue SynchQueue;
struct SynchQueue {
    QueueNode *head;
    QueueNode *tail;
    int size;
    void (*destroy_data)(void *);
};

//Function declarations
void enqueue(SynchQueue *, void *);
void *dequeue(SynchQueue *);

SynchQueue *queue_init(void (*destroy_data)(void *));
void queue_destroy(SynchQueue *);

QueueNode *create_queue_node(void *);
void *destroy_queue_node(QueueNode *);

//Helper functions
QueueNode *dequeue_node(SynchQueue *);

#define _SYNCH_QUEUE_H 1
#endif
