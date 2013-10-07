#include "ll.h"

LLNode *create_llnode(char *file_name)
{
    LLNode *n = malloc(sizeof(LLNode));
    if(n == NULL)
    {
        fprintf(stderr, "Malloc failed\n");
        return NULL;
    }

    n->file_name = file_name;

    return n;
}

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
