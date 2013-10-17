#include "tokenizer.h"

Tokenizer *create_tokenizer(char *filename, int(*is_delim)(char))
{
    FILE *fp;
    Tokenizer *t = malloc(sizeof(Tokenizer));

    if(t == NULL)
    {
        fprintf(stderr, "Malloc failed.\n");
        exit(1);
    }

    fp = fopen(filename, "r");
    if(fp == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", filename);
        exit(5);
    }

    t->fp = fp;
    t->is_delim = is_delim;

    return t;
}
