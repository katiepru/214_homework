#include "synch-queue.h"

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
