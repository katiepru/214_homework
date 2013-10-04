#include "sorted-list.h"

/*
 * SLCreate takes a pointer to a comparison function and returns a pointer to a
 * new SortedList struct which can maintain a list of data that the function
 * compares.
 */
SortedListPtr SLCreate(CompareFuncT cf){
    SortedListPtr list = malloc(sizeof(struct SortedList));

    /* check if malloc failed */
    if(!list) return NULL;

    list->compare = cf;
    list->head = NULL;

    return list;
}

/*
 * SLDestroy takes a pointer to a SortedList struct and frees all the nodes
 * pointed to by that list, followed by the struct itself.
 */
void SLDestroy(SortedListPtr list)
{
    /* Safety check */
    if(!list) return;

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

/*
 * SLInsert takes a pointer to a list object and pointer of an object of the
 * same type and attempts to insert it into the list. It returns 1 for
 * successful insert and 0 for failure. Failure can be caused by either pointer
 * being NULL or an object of the same value already existing in the list. In
 * the second case the object pointed to WILL NOT be freed, and should be
 * handled appropriately by the caller.
 */
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

    /* Check if the node should become the new head */
    if (ptr == NULL || list->compare(newObj, ptr->data) > 0)
    {
        newNode->next = ptr;
        list->head = newNode;
        IncNodeRef(newNode);
        return 1;
    }

    /* Check if the new object is the same as the head */
    if(!list->compare(newObj, ptr->data))
    {
        free(newNode);
        return 0;
    }

    next = ptr->next;

    /* Attempt to insert between two elements */
    while(next != NULL) {
        if(list->compare(newObj, next->data) > 0)
        {
            newNode->next = ptr->next;
            ptr->next = newNode;
            IncNodeRef(newNode);
            return 1;
        }
        /* Check for duplicate */
        else if (!list->compare(newObj, next->data))
        {
            free(newNode);
            return 0;
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

/*
 * IncNodeRef takes a pointer to a SortedListNode struct and increments its
 * reference count.
 */
void IncNodeRef(SortedListNodePtr node)
{
    node->references++;
}

/*
 * DecNodeRef takes a pointer to a SortedListNode struct and decrements its
 * reference count. If this causes the reference count to drop to 0 or below, it
 * recursively decrements the reference count on the node's next and then frees
 * the node.
 */
void DecNodeRef(SortedListNodePtr node)
{
    /* Safety check. */
    if(!node) return;

    node->references--;
    if (node->references <= 0)
    {
        DecNodeRef(node->next);
        SLDestroyNode(node);
    }
}

/*
 * SLDestroyNode takes a pointer to a SortedListNode struct. It frees the struct
 * and returns a pointer to its next.
 */
SortedListNodePtr SLDestroyNode(SortedListNodePtr node)
{
    SortedListNodePtr next;

    /* check if we were passed NULL */
    if (!node) return NULL;

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

    //Malloc failed
    if(!iter) return NULL;

    iter->index = NULL;
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

    if(iter->index == NULL)
    {
        iter->index = iter->list->head;

        //Head of list is NULL
        if(iter->index == NULL)
        {
            return NULL;
        }

        IncNodeRef(iter->list->head);
        return iter->index->data;
    }

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
