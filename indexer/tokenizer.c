#include "tokenizer.h"

/*
 * Allocates a tokenizer. Takes in a file name.
 */
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

/*
 * Frees a tokenizer and closes the file it is using
 */
void destroy_tokenizer(Tokenizer *t)
{
    if(t == NULL)
    {
        return;
    }

    fclose(t->fp);
    free(t);
}
