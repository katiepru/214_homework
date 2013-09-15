/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

typedef struct TokenizerT_
{
    char *tokens;
    int index;
    int len_tokens;
} TokenizerT;

/*
 * Calculate length of string, taking care of backslashes accordingly
 */
int calcStringLen(char *s)
{
    int len = 0;
    int i;

    for(i = 0; i < strlen(s); ++i)
    {
        if(!(s[i] == '/' && s[i+1] != '/'))
        {
            len++;
        }
    }
    return len;
}

char findSpecial(char c)
{
    switch(c)
    {
        case 'n':
            return '\n';
        case 't':
            return '\t';
        case 'v':
            return '\v';
        case 'b':
            return '\b';
        case 'r':
            return '\r';
        case 'f':
            return '\f';
        case 'a':
            return '\a';
    }
    return c;
}


/*
 * replaceSpecial takes a string and returns a copy of it with any special
 * escape sequences replaced by their ASCII value
 */
char* replaceSpecial(char* orig)
{
    char c;
    int i = 0;
    int new_len = 0;
    int orig_len = strlen(orig);
    char *parsed = malloc((sizeof(char)*orig_len)+1);

    for (c = orig[0]; c != '\0'; c=orig[++i])
    {
        if (c == '\\')
        {
            parsed[new_len] = findSpecial(orig[++i]);
        }
        else
        {
            parsed[new_len] = orig[i];
        }
        new_len++;
    }
    parsed[new_len] = '\0';
    parsed = realloc(parsed, sizeof(char) * new_len);
    return parsed;
}

void tokenize(TokenizerT* t, char* delim, char* str)
{
    int i;
    char* tokens;
    int len_tokens = 0;
    char ascii_bools[128];

    memset(ascii_bools, 0, 128);

    for (i = 0; i < strlen(delim); ++i)
    {
        ascii_bools[(int)delim[i]] = 1;
    }

    tokens = malloc((sizeof(char) * strlen(str))+1);

    for (i = 0; i < strlen(str); ++i)
    {
        if (ascii_bools[(int)str[i]])
        {
            /* only put a null byte if we have some tokens */
            if (len_tokens)
            {
                tokens[len_tokens++] = '\0';
            }
            while(ascii_bools[(int)str[i+1]] && str[i+1] != '\0')
            {
                i++;
            }
        }
        else
        {
            tokens[len_tokens++] = str[i];
        }
    }

    if (tokens[len_tokens] != '\0')
    {
        tokens[len_tokens] = '\0';
    }
    tokens = realloc(tokens, len_tokens);

    t->tokens = tokens;
    t->len_tokens = len_tokens;
    t->index = 0;
}

/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 *
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate(char *separators, char *ts)
{
    TokenizerT *t;
    char *separators_p;
    char *ts_p;

    t = malloc(sizeof(TokenizerT));
    if(t == NULL)
    {
        /*Malloc failed*/
        return NULL;
    }

    separators_p = replaceSpecial(separators);
    ts_p = replaceSpecial(ts);

    if (separators_p == NULL || ts_p == NULL)
    {
        return NULL;
    }

    tokenize(t, separators_p, ts_p);
    free(separators_p);
    free(ts_p);

    return t;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT *tk)
{
    free(tk->tokens);
    free(tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken(TokenizerT *tk)
{
    char *token;
    int length;

    if(tk->index >= tk->len_tokens)
    {
        return NULL;
    }

    length = strlen(&(tk->tokens[tk->index]));
    token = malloc(length * sizeof(char));
    strcpy(token, &(tk->tokens[tk->index]));
    tk->index += length + 1;

    return token;
}

/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv)
{
    TokenizerT *tk;
    char *token;

    /*Check that number of args is correct*/
    if(argc != 3)
    {
        fprintf(stderr, "Wrong number of arguements\n");
        return 1;
    }

    tk = TKCreate(argv[1], argv[2]);

    while((token = TKGetNextToken(tk)) != NULL)
    {
        printf("%s\n", token);
        free(token);
    }

    TKDestroy(tk);

    return 0;
}
