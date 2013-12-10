#include "my_malloc.h"

static char big_block[BLOCKSIZE];
static pthread_mutex_t my_malloc_mutex;

// return a pointer to the memory buffer requested
void* my_malloc(size_t size, const char *calling_file, const int calling_line)
{
    static int      initialized = 0;
    static struct   MemEntry *root;
    struct          MemEntry *p;
    struct          MemEntry *succ;
    struct          MemEntry *smallest;

    smallest = NULL;

    if(!initialized)    // 1st time called
    {
        // create a root chunk at the beginning of the memory block
        pthread_mutex_init(&my_malloc_mutex, NULL);
        root = (struct MemEntry*)big_block;
        root->prev = root->succ = 0;
        root->size = BLOCKSIZE - sizeof(struct MemEntry);
        root->isfree = 1;
        root->sig = BITSIG;
        initialized = 1;
    }
    pthread_mutex_lock(&my_malloc_mutex);
    p = root;
    do
    {
        if(p->isfree && p->size >= size && (smallest == NULL || p->size < smallest->size))
        {
            smallest = p;
        }
        p = p->succ;
    } while(p != 0);

    if (smallest == NULL)
    {
        // checked the entire pool, couldn't find a suitable memory block
        pthread_mutex_unlock(&my_malloc_mutex);
        return NULL;
    }
    else if(smallest->size < (size + sizeof(struct MemEntry)))
    {
        // this chunk is free and large enough to hold data,
        // but there's not enough memory to hold the HEADER of the next chunk
        // don't create any more chunks after this one
        smallest->isfree = 0;
        pthread_mutex_unlock(&my_malloc_mutex);
        return (char*)smallest + sizeof(struct MemEntry);
    }
    else
    {
        // take the needed memory and create the header of the next chunk
        succ = (struct MemEntry*)((char*)smallest + sizeof(struct MemEntry) + size);
        succ->prev = smallest;
        succ->succ = smallest->succ;
        succ->size = smallest->size - sizeof(struct MemEntry) - size;
        succ->sig  = BITSIG;
        succ->isfree = 1;
        smallest->size = size;
        smallest->isfree = 0;
        smallest->succ = succ;
        pthread_mutex_unlock(&my_malloc_mutex);
        return (char*)smallest + sizeof(struct MemEntry);
    }
}


// free a memory buffer pointed to by p
int my_free(void *p, const char *calling_file, const int calling_line)
{
    struct MemEntry *ptr;
    struct MemEntry *prev;
    struct MemEntry *succ;

    void *block_start = (void*)&big_block;
    void *block_end = (&big_block + BLOCKSIZE - sizeof(struct MemEntry) - 1);

    pthread_mutex_lock(&my_malloc_mutex);
    ptr = (struct MemEntry*)((char*)p - sizeof(struct MemEntry));

    if ((void*)ptr < block_start || (void*)ptr >= block_end)
    {
        // they asked us to free something outside of our memory pool
        fprintf(stderr, "ERROR: calling free on pointer outside bounds of memory pool. %s:%d\n", calling_file, calling_line);
        pthread_mutex_unlock(&my_malloc_mutex);
        return 1;
    }
    if (ptr->sig != BITSIG)
    {
        // the bit signature isn't here. must have been givena bad pointer
        fprintf(stderr, "ERROR: calling free on pointer not returned from malloc. %s:%d\n", calling_file, calling_line);
        pthread_mutex_unlock(&my_malloc_mutex);
        return 1;
    }
    if (ptr->isfree)
    {
        //the block was alread freed. report error
        fprintf(stderr, "ERROR: calling free on already freed block. %s:%d\n", calling_file, calling_line);
        pthread_mutex_unlock(&my_malloc_mutex);
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

    pthread_mutex_unlock(&my_malloc_mutex);
    return 0;
}

void _print_heap()
{
    int i=0;
    struct MemEntry *p;

    p = (struct MemEntry*)big_block;
    fprintf(stderr, "\n==================\n");
    do
    {
        fprintf(stderr, "|id:       %d\n"
                        "|size:     %d\n"
                        "|free:     %c\n"
                        "|prev:     %ld\n"
                        "|succ:     %ld\n"
                        "==================\n", i, p->size, p->isfree + 48, (long int)p->prev, (long int)p->succ);
        ++i;
        p = p->succ;
    } while(p != 0);
    fprintf(stderr, "\n");
}
