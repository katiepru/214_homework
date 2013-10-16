#include "indexer.h"

int main(int argc, char *argv[])
{
    //Check that we have the right number of args
    if(argc < 3)
    {
        fprintf(stderr, "Please provide the output file and directory to index\n");
        return 1;
    }
    if(argc > 3)
    {
        fprintf(stderr, "Please only provide the output file and directory to index\n");
        return 1;
    }
    return 0;
}

/*
 * String compare function
 * Works backwards to get list to work with ascending order
 */
int compareString(void *p1, void *p2)
{
    char *s1 = p2;
    char *s2 = p1;

    return strcmp(s1, s2);
}
