#include "my_malloc.h"
#include <stdio.h>
#include <string.h>

#define malloc(x) my_malloc(x, __FILE__, __LINE__)
#define free(x) my_free(x, __FILE__, __LINE__)

int main(int argc, char *argv[])
{
    int i;

    char *mystr = malloc(sizeof(char) * 10);
    for (i = 0; i < 10; ++i)
    {
        mystr[i] = (char) (97 + i);
    }
    mystr[9] = '\0';
    if (strcmp(mystr, "abcdefghi") != 0)
    {
        fprintf(stderr, "[31mERROR: reading or writing malloced memory failed.[m\n");
        return 1;
    }
    else
    {
        fprintf(stderr, "[32mBasic r/w with malloced memory works.[m\n");
    }

    return 0;
}
