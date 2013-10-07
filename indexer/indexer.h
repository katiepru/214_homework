#ifndef _INDEXER_H

#include <stdio.h>
#include <stdlib.h>
#include "file_info.h"

//Struct definitions
typedef struct TermNode {
    char *term;
    struct TermNode *next;
    FileInfo *file_info;
} TermNode;

#define _INDEXER_H 1
#endif
