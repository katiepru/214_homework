/*
 * sorted-list.c
 */

#include	<string.h>
#include    <stdio.h>
#include	"sorted-list.h"

int compareInts(void *p1, void *p2)
{
	int i1 = *(int*)p1;
	int i2 = *(int*)p2;

	return i1 - i2;
}

int compareDoubles(void *p1, void *p2)
{
	double d1 = *(double*)p1;
	double d2 = *(double*)p2;

	return (d1 < d2) ? -1 : ((d1 > d2) ? 1 : 0);
}

int compareStrings(void *p1, void *p2)
{
	char *s1 = p1;
	char *s2 = p2;

	return strcmp(s1, s2);
}

int main()
{
    int i;
    void *returned;

    //Test ints
    SortedListIteratorPtr iter;
    SortedListIteratorPtr iter2;
    SortedListPtr intList = SLCreate(compareInts);
    int ints[10];
    int *intPtrs[10];
    int result;
    int tmp;

    // Set up list
    for(i = 0; i < 10; ++i)
    {
        ints[i] = 2*i;
        intPtrs[i] = &(ints[i]);
        SLInsert(intList, (void *) intPtrs[i]);
    }

    // Set up iterator
    iter = SLCreateIterator(intList);

    //Iterate through the list and make sure values are correct
    i = 9;
    while((returned = SLNextItem(iter)) != NULL)
    {
        if((result = *((int *) returned)) != 2 * i)
        {
            fprintf(stderr, "Got %d, expected %d\n", result, 2*i);
            return 1;
        }
        --i;
    }

    //Destroy iterator
    SLDestroyIterator(iter);

    //Create new iterators and advance them by 2
    iter = SLCreateIterator(intList);
    iter2 = SLCreateIterator(intList);
    SLNextItem(iter);
    SLNextItem(iter);
    SLNextItem(iter2);
    SLNextItem(iter2);

    //Destroy node we are on
    i = 16;
    SLRemove(intList, (void *) &i);

    //Insert node that should have gone after original
    tmp = 15;
    SLInsert(intList, (void *) &tmp);

    //Get next item from iter
    returned = SLNextItem(iter);

    //Check that we got 15
    if((result = *((int *) returned)) != 15)
    {
        fprintf(stderr, "Got %d, expected 15\n", result);
        return 2;
    }

    //Delete 15
    SLRemove(intList, (void *) &tmp);

    //Add something that should have gone before original
    tmp = 17;
    SLInsert(intList, (void *) &tmp);

    //Get next from iter2
    returned = SLNextItem(iter2);

    //Make sure it is 14
    if((result = *((int *) returned)) != 14)
    {
        fprintf(stderr, "Got %d, expected 14\n", result);
        return 2;
    }

    //Destroy iterators
    SLDestroyIterator(iter);
    SLDestroyIterator(iter2);

    //Destroy list
    SLDestroy(intList);

    return 0;
}
