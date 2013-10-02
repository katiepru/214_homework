#include "sorted-list.h"

SortedListPtr SLCreate(CompareFuncT cf){
    SortedListPtr list = malloc(sizeof(struct SortedList));

    /* check if malloc failed */
    if(!list) return NULL;

    list->compare = cf;
    list->head = NULL;

    return list;
}

void SLDestroy(SortedListPtr list)
{
    SortedListNodePtr ptr = list->head;
    SortedListNodePtr next;

    while(ptr != NULL)
    {
        free(ptr->data);
        next = ptr->next;
        SLDestroyNode(ptr);
        ptr=next;
    }
}

int SLInsert(SortedListPtr list, void *newObj)
{
    SortedListNodePtr ptr;
    SortedListNodePtr next;
    SortedListNodePtr newNode;

    /* did they pass us NULL? */
    if (!newObj || !list)
    {
        return 0;
    }

    newNode = malloc(sizeof(struct SortedListNode));
    newNode->data = newObj;

    ptr = list->head;

    /* If the list is empty, make this node the head and return. */
    if (ptr == NULL)
    {
        list->head = newNode;
        IncNodeRef(newNode);
        return 1;
    }

    /* Check if the node should become the new head */
    if (list->compare(newObj, ptr->data) <= 0)
    {
        newNode->next = ptr;
        list->head = newNode;
        IncNodeRef(newNode);
        return 1;
    }

    next = ptr->next;

    while(next != NULL) {
        if(list->compare(newObj, ptr->data) >= 1
        && list->compare(newObj, next->data) <= 0)
        {
            newNode->next = ptr->next;
            ptr->next = newNode;
            IncNodeRef(newNode);
            return 1;
        }
        ptr = next;
        next = next->next;
    }

    /* Reached end of list, node goes in as last element. */
    newNode->next = NULL;
    ptr->next = newNode;
    IncNodeRef(newNode);
    return 1;
}

void IncNodeRef(SortedListNodePtr node)
{
    node->references++;
}

void DecNodeRef(SortedListNodePtr node)
{
    node->references--;
    if (node->references <= 0)
    {
        SLDestroyNode(node);
    }
}

SortedListNodePtr SLDestroyNode(SortedListNodePtr node)
{
    SortedListNodePtr next;

    /* check if we were passed NULL */
    if (!node)
    {
        return NULL;
    }

    next = node->next;

    free(node->data);
    free(node);

    return next;
}

/*
 * Creates a SortedListIterator that points to the head of a list
 */
SortedListIteratorPtr SLCreateIterator(SortedListPtr list)
{
    SortedListIteratorPtr iter = malloc(sizeof(struct SortedListIterator));

    if(iter == NULL)
    {
        //Malloc failed
        return NULL;
    }

    iter->index = list->head;
    iter->list = list;
    return iter;
}

/*
 * Destroys a SortedListIterator
 */
void SLDestroyIterator(SortedListIteratorPtr iter)
{
    DecNodeRef(iter->index);
    free(iter);
}

/*
 * Advances iterator and return data at node
 */
void *SLNextItem(SortedListIteratorPtr iter)
{
    void *curr_val = iter->index->data;
    SortedListNodePtr tmp;

    //Check if we got deleted from list
    if(iter->index->references == 0)
    {
        //Start from head and return first item that is smaller
        curr_val = iter->index->data;
        tmp = iter->list->head;
        while(tmp != NULL &&
              (*iter->list->compare)(tmp->data, curr_val) > 0)
        {
            tmp = tmp->next;
        }

        DecNodeRef(iter->index);
        iter->index = tmp;
        IncNodeRef(iter->index);
    }
    else
    {
        //Reached end of list
        tmp = iter->index->next;
        if(tmp == NULL)
        {
            SLDestroyIterator(iter);
            return NULL;
        }

        //Adjust references accordingly
        iter->index = tmp->next;
        DecNodeRef(tmp);
        IncNodeRef(iter->index);
    }

    return iter->index->data;
}
