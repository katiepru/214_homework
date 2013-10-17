#include "tokenizer.h"

char *get_next_token(Tokenizer *t)
{
    char *token = calloc(100, sizeof(char));
    int len = 100;
    int i = 0;

    char c = fgetc(t->fp);
    while(c != EOF && !t->is_delim(c))
    {
        if(i >= len)
        {
            token = realloc(token, len + 100);
            len += 100;
        }

        token[i] = c;
        i++;

        c = fgetc(t->fp);
    }

    if(i == len)
    {
        token = realloc(token, len + 1);
        token[i] = 0;
    }
    else
    {
        token[i] = 0;
        token = realloc(token, strlen(token));
    }

    return token;
}

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
