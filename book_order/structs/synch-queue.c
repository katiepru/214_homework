#include "synch-queue.h"

//Queue use functions
void enqueue(SynchQueue *q, void *data)
{
    QueueNode *n = create_queue_node(data);

    if(n == NULL) return;

    //Handle empty queue
    if(q->tail == NULL)
    {
        q->head = n;
        q->tail = n;
        q->size = 1;
    }
    else
    {
        q->tail->next = n;
        q->tail = n;
        q->size++;
    }
}

void *dequeue(SynchQueue *q)
{
    QueueNode *n = dequeue_node(q);
    void *data;

    if(n == NULL)
    {
        return NULL;
    }

    data = n->data;

    destroy_queue_node(n);

    return data;
}

QueueNode *dequeue_node(SynchQueue *q)
{
    QueueNode *ret;

    //Empty
    if(q->size == 0)
    {
        return NULL;
    }

    ret = q->head;

    //One entry
    if(q->size == 1)
    {
        q->head = NULL;
        q->tail = NULL;
    }
    //Multiple entries
    else
    {
        q->head = ret->next;
    }
    q->size--;

    return ret;
}

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
