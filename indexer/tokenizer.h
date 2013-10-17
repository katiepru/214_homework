#ifndef _TOKENIZER_H

#include <stdlib.h>
#include <stdio.h>

typedef struct Tokenizer Tokenizer;

struct Tokenizer {
    FILE *fp;
    int (*is_delim)(char c);
};

//Function declarations
Tokenizer *create_tokenizer(char *, int(*)(char));

#define _TOKENIZER_H 1
#endif
