#include "sorted-list.h"

SortedListPtr SLCreate(CompareFuncT cf){
    SortedListPtr list = malloc(sizeof(struct SortedList));

    /* check if malloc failed */
    if(!list) return NULL;

    list->compare = cf;
    list->head = NULL;

    return list;
}
