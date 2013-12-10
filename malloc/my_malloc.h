#ifndef _MY_MALLOC

#include <stdio.h>
#include <pthread.h>

// Size of char array to allocate and use as memory pool
#define BLOCKSIZE   100000
// Random number to check if we're actually pointing to a MemEntry
#define BITSIG      295723045

// Redefine malloc and free when someone includes our header
#define malloc(x) my_malloc(x, __FILE__, __LINE__)
#define free(x) my_free(x, __FILE__, __LINE__)


void* my_malloc(size_t, const char*, const int);

int my_free(void*, const char*, const int);

void _print_heap();

struct MemEntry
{
    struct  MemEntry *prev, *succ;
    int     sig;
    int     isfree;     // 1 - yes, 0 - no
    int     size;
};

#define _MY_MALLOC 1
#endif
