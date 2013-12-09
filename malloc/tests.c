#include "my_malloc.h"
#include <stdio.h>
#include <string.h>

#define malloc(x) my_malloc(x, __FILE__, __LINE__)
#define free(x) my_free(x, __FILE__, __LINE__)

int main(int argc, char *argv[])
{
    int i;

    /*****************************
    *  Test basic malloc usage  *
    *****************************/
    char *mystr = malloc(sizeof(char) * 10);
    for (i = 0; i < 10; ++i)
    {
        mystr[i] = (char) (97 + i);
    }
    mystr[9] = '\0';
    if (strcmp(mystr, "abcdefghi") != 0)
    {
        printf("[31mERROR: reading or writing malloced memory failed.[m\n");
        return 1;
    }
    else
    {
        printf("[32mBasic r/w with malloced memory works.[m\n");
    }

    /***************************
    *  Test basic free usage  *
    ***************************/
    if (free(mystr) == 0)
    {
        printf("[32mBasic use of free works.[m\n");
    }
    else
    {
        printf("[31mERROR: basic use of free failed.[m\n");
        return 1;
    }

    /*********************************************
    *  Test for error reporting on double free  *
    *********************************************/
    if (free(mystr) == 1)
    {
        printf("[32mError checking for double free works.[m\n");
    }
    else
    {
        printf("[31mERROR: checking for double free fails.[m\n");
        return 1;
    }

    return 0;
}
