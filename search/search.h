#include "search.h"

int main(int argc, char **argv)
{
    FILE *input;

    //Checl number of arguments
    if(argc != 2)
    {
        fprintf(stderr, "ERROR: Wrong number of arguments.\n");
        return 1;
    }

    //Open file
    input = fopen(argv[1], "r");

    //Check that it worked
    if(input == NULL)
    {
        fprintf(stderr, "Unable to open file %s.\n", argv[1]);
        return 2;
    }

}
