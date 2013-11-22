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
    QueueNode *rear;
    int size;
    void (*destroy_data)(void *);
};

//Function declarations
QueueNode *create_queue_node(void *);
void *destroy_queue_node(QueueNode *);

#define _SYNCH_QUEUE_H 1
#endif
