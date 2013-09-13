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
	char *separators;
	char *ts;
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
	int len = calcStringLen(separators);

    t = malloc(sizeof(TokenizerT));
	if(t == NULL)
	{
		/*Malloc failed*/
		return NULL;
	}

	t->separators = malloc(sizeof(*separators));
	t->ts = malloc(sizeof(*ts));

    if(t->separators == NULL || t->ts == NULL)
    {
        /*Malloc failed*/
        return NULL;
    }

    strcpy(t->separators, separators);
    strcpy(t->ts, ts);

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
    if(tk->separators != NULL)
    {
        free(tk->separators);
    }

    if(tk->ts != NULL)
    {
        free(tk->ts);
    }

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

  return NULL;
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
	if(argc != 2)
	{
		fprintf(stderr, "Wrong number of arguements\n");
		return 1;
	}

	tk = TKCreate(argv[1], argv[2]);
	while((token = TKGetNextToken(tk)) != NULL)
	{
		printf("%s\n", token);
	}

 	return 0;
}
