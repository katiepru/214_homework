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

    while(ptr != NULL)
    {
        ptr = SLDestroyNode(ptr);
    }
    free(list);
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
    newNode->deleted = 0;
    newNode->references = 0;

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

    free(node);

    return next;
}

/*
 * Creates a SortedListIterator that points to the head of a list
 */
SortedListIteratorPtr SLCreateIterator(SortedListPtr list)
{
    /* Safety check */
    if(!list) return NULL;

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
 * Advance iterator and return data at node
 */
void *SLNextItem(SortedListIteratorPtr iter)
{
    /* Safety check */
    if(!iter) return NULL;

    SortedListNodePtr tmp;
    void* curr_val;

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
    if(iter->index->deleted)
    {
        //Start from head and return first item that is smaller
        curr_val = iter->index->data;
        tmp = iter->list->head;
        while(tmp != NULL && (*iter->list->compare)(tmp->data, curr_val) >= 0)
        {
            tmp = tmp->next;
        }
    }
    else
    {
        /* Still in the list, go to next element */
        tmp = iter->index->next;
    }

    //Reached end of list
    if(tmp == NULL)
    {
        return NULL;
    }

    //Adjust references
    DecNodeRef(iter->index);
    iter->index = tmp;
    IncNodeRef(iter->index);

    return iter->index->data;
}


void *SLPeekItem(SortedListIteratorPtr iter)
{
    /* Safety check */
    if(!iter) return NULL;

    /* do we have the head yet? */
    if(iter->index == NULL)
    {
        //Head of list is NULL
        if(iter->list->head == NULL)
        {
            return NULL;
        }

        return iter->list->head->data;
    }

    //Reached end of list
    if (iter->index->next == NULL)
    {
        return NULL;
    }

    return iter->index->next->data;
}

/*
 * SLRemove removes an item from a sorted list
 */
int SLRemove(SortedListPtr list, void *newObj)
{
    /* Safety check */
    if(!list || !newObj) return 0;

    int comp_result;
    SortedListNodePtr previous;
    SortedListNodePtr ptr = list->head;

    /* see if we have an empty list or newObj can't be in the list */
    if(ptr == NULL || list->compare(newObj, ptr->data) >  0)
    {
        return 0;
    }

    /* see if it's the head */
    if(!list->compare(newObj, ptr->data))
    {
        list->head = ptr->next;
        IncNodeRef(ptr->next);
        DecNodeRef(ptr);
        ptr->deleted = 1;
        return 1;
    }

    previous = ptr;
    ptr = ptr->next;

    /* search through the list for value */
    while(ptr != NULL)
    {
        comp_result = list->compare(newObj, ptr->data);

        /* found object, remove it */
        if(comp_result == 0)
        {
            previous->next = ptr->next;
            IncNodeRef(ptr->next);
            ptr->deleted = 1;
            DecNodeRef(ptr);
            return 1;
        }

        previous = ptr;
        ptr = ptr->next;
    }

    /* reached the end, didn't find the object */
    return 0;
}
