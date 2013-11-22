#include "synch-queue.h"

//SynchQueue memory management

SynchQueue *queue_init(void (*destroy_data)(void *data))
{
    SynchQueue *q = malloc(sizeof(SynchQueue));

    if(q == NULL)
    {
        fprintf(stderr, "Malloc failed.\n");
        return NULL;
    }

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    q->destroy_data = destroy_data;

    return q;
}

void queue_destroy(SynchQueue *q)
{
    QueueNode *n;
    void *ret;

    while((n = dequeue_node(q)) != NULL)
    {
        ret = destroy_queue_node(n);
        q->destroy_data(ret);
    }

    free(q);
}

//QueueNode memory management

QueueNode *create_queue_node(void *data)
{
    QueueNode *n = malloc(sizeof(QueueNode));

    if(n == NULL)
    {
        fprintf(stderr, "Malloc failed.\n");
        return NULL;
    }

    n->data = data;
    n->next = NULL;

    return n;
}

void *destroy_queue_node(QueueNode *n)
{
    void *data = n->data;
    free(n);
    return data;
}
