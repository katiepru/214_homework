#include "file-node.h"

/*
 * Creates a node containing file name and wordcount for this word
 */
FileNode *create_filenode(char *file_name)
{
    FileNode *f = malloc(sizeof(FileNode));

    if(f == NULL)
    {
        fprintf(stderr, "Malloc failed.\n");
        exit(1);
    }

    f->file_name = file_name;
    f->count = 1;

    return f;
}

/*
 * Frees a file node
 */
void destroy_filenode(FileNode *f)
{
    free(f->file_name);
    free(f);
}
