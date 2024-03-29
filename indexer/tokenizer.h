#ifndef _TOKENIZER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Tokenizer Tokenizer;

struct Tokenizer {
    FILE *fp;
    int (*isDelim)(char c);
};

//Function declarations
Tokenizer *create_tokenizer(char *, int(*)(char));
void destroy_tokenizer(Tokenizer *);
char *get_next_token(Tokenizer *t);

int isDelim(char);

#define _TOKENIZER_H 1
#endif
