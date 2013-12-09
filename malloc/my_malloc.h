#ifndef _MY_MALLOC

#include <stdio.h>

// Size of char array to allocate and use as memory pool
#define BLOCKSIZE   100000

static char big_block[BLOCKSIZE];

void* my_malloc(unsigned int, const char*, const int);

int my_free(void*, const char*, const int);

struct MemEntry
{
    struct MemEntry *prev, *succ;
    int     isfree;     // 1 - yes, 0 - no
    int     size;
};

#define _MY_MALLOC 1
#endif
