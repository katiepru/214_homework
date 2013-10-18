#ifndef _INDEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs/trie.h"
#include "structs/sorted-list.h"
#include "structs/ll.h"
#include "structs/file-node.h"

//Function declarations
int compare_file_nodes(void *, void *);
int compareStrings(void *, void *);
int isDelim(char);
void insert_into_master(char *, char *, void *, void *);
void insert_data_sorted_lsit(TrieNode *, void *);
void insert_data(TrieNode *, void *);
void destroy_data(void *);

#define _INDEXER_H 1
#endif
