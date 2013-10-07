#ifndef _LL_H

//Includes
#include <stdlib.h>
#include <stdio.h>

//Struct definition
typedef struct LLNode {
    char *file_name;
    int count;
    struct LLNode *next;
} LLNode;

#define _LL_H 1
#endif
