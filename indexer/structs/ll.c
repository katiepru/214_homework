#include "ll.h"

/*
 * Allocates a list
 */
LL *create_list()
{
    LL *list = malloc(sizeof(LL));

    if(list == NULL)
    {
        fprintf(stderr, "Malloc failed.\n");
        exit(1);
    }

    list->head = NULL;
    list->curr = NULL;

    return list;
}

/*
 * Allocates a node
 */
LLNode *create_llnode(char *file_name, LL *list)
{
    LLNode *n = malloc(sizeof(LLNode));
    if(n == NULL)
    {
        fprintf(stderr, "Malloc failed\n");
        return NULL;
    }

    n->file_name = file_name;
    n->list = list;

    return n;
}

/*
 * Frees a list
 * Assumes list is not NULL
 */
void destroy_list(LL* list)
{
    destroy_llnode(list->head);
    free(list);
}

/*
 * Frees a node and all of it's children
 */
void destroy_llnode(LLNode *n)
{
    if(n == NULL)
    {
        return;
    }

    if(n->next != NULL)
    {
        destroy_llnode(n->next);
    }

    free(n->file_name);
    free(n);
}
