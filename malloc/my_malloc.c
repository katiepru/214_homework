#include "my_malloc.h"

// return a pointer to the memory buffer requested
void* my_malloc(unsigned int size, const char *calling_file, const int calling_line)
{
    static int      initialized = 0;
    static struct MemEntry *root;
    struct MemEntry *p;
    struct MemEntry *succ;

    if(!initialized)    // 1st time called
    {
        // create a root chunk at the beginning of the memory block
        root = (struct MemEntry*)big_block;
        root->prev = root->succ = 0;
        root->size = BLOCKSIZE - sizeof(struct MemEntry);
        root->isfree = 1;
        root->sig = BITSIG;
        initialized = 1;
    }
    p = root;
    do
    {
        if(p->size < size)
        {
            // the current chunk is smaller, go to the next chunk
            p = p->succ;
        }
        else if(!p->isfree)
        {
            // this chunk was taken, go to the next
            p = p->succ;
        }
        else if(p->size < (size + sizeof(struct MemEntry)))
        {
            // this chunk is free and large enough to hold data,
            // but there's not enough memory to hold the HEADER of the next chunk
            // don't create any more chunks after this one
            p->isfree = 0;
            return (char*)p + sizeof(struct MemEntry);
        }
        else
        {
            // take the needed memory and create the header of the next chunk
            succ = (struct MemEntry*)((char*)p + sizeof(struct MemEntry) + size);
            succ->prev = p;
            succ->succ = p->succ;
            succ->size = p->size - sizeof(struct MemEntry) - size;
            succ->sig  = BITSIG;
            p->size = size;
            p->isfree = 0;
            p->succ = succ;
            return (char*)p + sizeof(struct MemEntry);
        }
    } while(p != 0);

    // checked the entire pool, couldn't find a suitable memory block
    return NULL;
}


// free a memory buffer pointed to by p
int my_free(void *p, const char *calling_file, const int calling_line)
{
    struct MemEntry *ptr;
    struct MemEntry *prev;
    struct MemEntry *succ;

    ptr = (struct MemEntry*)((char*)p - sizeof(struct MemEntry));

    if ((void*)ptr < (void*)&big_block || (void*)ptr >= (void*)(&big_block + BLOCKSIZE - sizeof(struct MemEntry) - 1))
    {
        // they asked us to free something outside of our memory pool
        fprintf(stderr, "ERROR: calling free on pointer outside bounds of memory pool. %s:%d\n", calling_file, calling_line);
        return 1;
    }
    if (ptr->sig != BITSIG)
    {
        // the bit signature isn't here. must have been givena bad pointer
        fprintf(stderr, "ERROR: calling free on pointer not returned from malloc. %s:%d\n", calling_file, calling_line);
        return 1;
    }
    if (ptr->isfree)
    {
        //the block was alread freed. report error
        fprintf(stderr, "ERROR: calling free on already freed block. %s:%d\n", calling_file, calling_line);
        return 1;
    }
    if ((succ = ptr->succ) != 0 && succ->isfree)
    {
        // succ is free, merge it into this block
        ptr->size +=  sizeof(struct MemEntry) + succ->size;
        ptr->succ = succ->succ;
    }
    if((prev = ptr->prev) != 0 && prev->isfree)
    {
        // the previous chunk is free, so
        // merge this chunk with the previous chunk
        prev->size += sizeof(struct MemEntry) + ptr->size;
        prev->succ = ptr->succ;
    }
    // do this anyway for the error checks up above
    ptr->isfree = 1;

    return 0;
}
