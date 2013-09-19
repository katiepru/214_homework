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
 * findSpecial takes a character and returns it's corresponding
 * backslash-escaped value if one exists. Otherwise just returns the original
 * input. If the input character is a null byte, this indicates there was a
 * trailing backslash and causes the program to exit with an error message.
 */

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
		case '\0':
			fprintf(stderr, "Bad input string. Cannot end in singular backslash.\n");
			exit(2);
    }
    return c;
}


/*
 * replaceSpecial takes a string and returns a copy of it with any special
 * escape sequences replaced by their ASCII value
 */

char* replaceSpecial(char* str)
{
    char c;
    int i = 0;
    int new_len = 0;

    for (c = str[0]; c != '\0'; c=str[++i])
    {
        if (c == '\\')
        {
            str[new_len] = findSpecial(str[++i]);
        }
        else
        {
            str[new_len] = str[i];
        }
        new_len++;
    }

    if (new_len < i)
    {
        str[new_len] = '\0';
        str = realloc(str, sizeof(char) * new_len);
    }
    return str;
}


/*
 * Given a pointer to a TokenizerT struct and two strings, tokenize tokenizes
 * the second string using the first as a set of delimiters and stores the
 * result in the TokenizerT struct.
 */

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

    tokens[len_tokens] = '\0';
    tokens = realloc(tokens, sizeof(char)*(len_tokens+1));

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

    separators_p = replaceSpecial(strcpy(malloc(strlen(separators)+1),
                                         separators));
    ts_p = replaceSpecial(strcpy(malloc(strlen(ts)+1), ts));

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
    token = malloc((length * sizeof(char))+1);
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
    int i;

    /*Check that number of args is correct*/
    if(argc != 3)
    {
        fprintf(stderr, "Wrong number of arguements\n");
        return 1;
    }

    tk = TKCreate(argv[1], argv[2]);

    if (tk == NULL)
    {
        fprintf(stderr, "TKCreate returned null.\n");
        return 1;
    }

    while((token = TKGetNextToken(tk)) != NULL)
    {
        for(i = 0; i < strlen(token); ++i)
        {
            switch(token[i])
            {
                case '\n':
                case '\t':
                case '\v':
                case '\b':
                case '\r':
                case '\a':
                    printf("[0x%.2x]",token[i]);
                    break;
                default:
                    printf("%c", token[i]);
            }
        }
        printf("\n");
        free(token);
    }

    TKDestroy(tk);

    return 0;
}
