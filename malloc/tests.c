#include "my_malloc.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int i;

    char *mystr = my_malloc(sizeof(char) * 10);
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
