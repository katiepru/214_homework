#ifndef _FILE_NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct FileNode {
    char *file_name;
    int count;
};

typedef struct FileNode FileNode;

FileNode *create_filenode(char *file_name);
void destroy_filenode(FileNode *);

#define _FILE_NODE_H 1
#endif
