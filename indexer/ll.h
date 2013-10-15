#ifndef _LL_H

//Includes
#include <stdlib.h>
#include <stdio.h>

//Struct definition
typedef struct LLNode LLNode;
typedef struct LL LL;

struct LLNode {
    char *file_name;
    int count;
    LLNode *next;
    LL *list;
};

struct LL {
    LLNode *head;
    LLNode *curr;
};

//Function declarations
LL *create_list();
LLNode *create_llnode(char *, LL *);
void destroy_list(LL *);
void destroy_llnode(LLNode *);

#define _LL_H 1
#endif
