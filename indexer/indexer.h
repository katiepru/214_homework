#ifndef _INDEXER_H

#include <stdio.h>
#include <stdlib.h>

//Struct definitions
typedef struct TermNode {
    char *term;
    struct TermNode *next;
} TermNode;

#define _INDEXER_H 1
#endif
