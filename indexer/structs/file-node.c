#include "file-node.h"

/*
 * Creates a node containing file name and wordcount for this word
 */
FileNode *create_filenode(char *file_name)
{
    FileNode *f = malloc(sizeof(FileNode));
    char *new_filename;

    if(f == NULL)
    {
        fprintf(stderr, "Malloc failed.\n");
        exit(1);
    }

    strcpy((new_filename = malloc(sizeof(char) * (strlen(file_name) + 1))), file_name);
    f->file_name = new_filename;
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
