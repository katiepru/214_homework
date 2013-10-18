#ifndef _FILE_NODE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct FileNode FileNode;

struct FileNode {
    char *file_name;
    int count;
};

FileNode *create_filenode(char *file_name);
void destroy_filenode(FileNode *);

#define FILE_NODE_H 1
#endif
