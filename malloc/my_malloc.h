#ifndef _MY_MALLOC

// Size of char array to allocate and use as memory pool
#define BLOCKSIZE   100000

void* my_malloc(unsigned int);

int my_free(void*);

struct MemEntry
{
    struct MemEntry *prev, *succ;
    int     isfree;     // 1 - yes, 0 - no
    int     size;
};

#define _MY_MALLOC 1
#endif
