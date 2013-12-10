#include "my_malloc.c"
#include <stdio.h>
#include <string.h>

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

    /**************************************************
    *  Test for freeing pointer not in memory pool.  *
    **************************************************/
    if (free(&big_block) == 1)
    {
        printf("[32mError checking for freeing pointer before memory pool works.[m\n");
    }
    else
    {
        printf("[31mERROR: checking for freeing pointer before memory pool fails.[m\n");
        return 1;
    }
    if (free(&big_block + BLOCKSIZE) == 1)
    {
        printf("[32mError checking for freeing pointer after memory pool works.[m\n");
    }
    else
    {
        printf("[31mERROR: checking for freeing pointer after memory pool fails.[m\n");
        return 1;
    }

    /****************************************************************************
    *  Test calling free on pointer inside pool but not returned from malloc.  *
    ****************************************************************************/

    if (free(mystr + 10) == 1)
    {
        printf("[32mError checking calling free on pointer not returned from malloc works.[m\n");
    }
    else
    {
        printf("[31mERROR: calling free on pointer not returned from malloc fails.[m\n");
        return 1;
    }

    /*******************************************
    *  Test running out of available memory.  *
    *******************************************/
    mystr = malloc(BLOCKSIZE - sizeof(struct MemEntry));
    if (mystr == NULL)
    {
        printf("[31mERROR: An unknown error occured while testing for out of memory.[m\n");
        return 1;
    }

    if (malloc(100) == NULL)
    {
        printf("[32mDetecting out of memory works.[m\n");
    }
    else
    {
        printf("[31mDetecting out of memory fails.[m\n");
        return 1;
    }
    free(mystr);

    /*************************************
    *  Test for reducing fragmentation  *
    *************************************/
    // malloc and free to create one large free chunk at the start of the
    // pool and one smaller free chunk at the end
    char *first_half = malloc(BLOCKSIZE / 2 - sizeof( struct MemEntry ));
    mystr = malloc( BLOCKSIZE / 4 - sizeof( struct MemEntry ) );
    free(first_half);

    // attempt to allocate a small ammount that will fit in the space at the end
    char *small_block = malloc(BLOCKSIZE / 4 - sizeof( struct MemEntry ));

    // then try to mallock a larger block that would only fit in the first half
    char *large_block = malloc(BLOCKSIZE / 2 - sizeof( struct MemEntry ));

    if (small_block == NULL)
    {
        printf("[31mERROR: Failed to allocate small block.[m\n");
        return 1;
    }
    if (large_block == NULL)
    {
        printf("[31mERROR: Failed to allocate small block.[m\n");
        return 1;
    }
    if (malloc(BLOCKSIZE / 3) != NULL)
    {
        printf("[31mERROR: An unknown error occured in the fragmentation test.[m\n");
        return 1;
    }
    printf("[32mPreventing fragmentation works.[m\n");

    return 0;
}
