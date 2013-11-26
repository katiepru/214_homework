#include "synch-queue.h"

//Queue use functions

/*
 * Enqueue an item
 * Locks queue during this process
 */
void enqueue(SynchQueue *q, void *data)
{
    QueueNode *n = create_queue_node(data);

    if(n == NULL) return;

    pthread_mutex_lock(&(q->mutex));

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

    pthread_mutex_unlock(&(q->mutex));
}

/*
 * Dequeue an item
 * Locks the queue during this process
 */
void *dequeue(SynchQueue *q)
{
    pthread_mutex_lock(&(q->mutex));

    QueueNode *n = dequeue_node(q);
    void *data;

    if(n == NULL)
    {
        pthread_mutex_unlock(&(q->mutex));
        return NULL;
    }

    data = n->data;

    destroy_queue_node(n);

    pthread_mutex_unlock(&(q->mutex));

    return data;
}

/*
 * Dequeue a QueueNode
 * Not thread safe - doesn't need to be
 */
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

/*
 * Malloc and initialize a queue
 */
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

    sem_init(&(q->semaphore), 0, 0);
    pthread_mutex_init(&(q->mutex), NULL);

    return q;
}

/*
 * Free a queue
 */
void queue_destroy(SynchQueue *q)
{
    QueueNode *n;
    void *ret;

    if(q == NULL) return;

    while((n = dequeue_node(q)) != NULL)
    {
        ret = destroy_queue_node(n);
        q->destroy_data(ret);
    }

    sem_destroy(&(q->semaphore));
    pthread_mutex_destroy(&(q->mutex));

    free(q);
}

//QueueNode memory management

/*
 * Malloc and initialize a queue node
 */
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

/*
 * Free a queue node
 */
void *destroy_queue_node(QueueNode *n)
{
    void *data = n->data;
    free(n);
    return data;
}
