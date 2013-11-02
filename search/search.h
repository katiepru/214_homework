#ifndef _SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include "structs/trie.h"
#include "structs/file-node.h"
#include "structs/sorted-list.h"

//Function Declarations
Trie *preprocess_file(FILE *);

//Helper functions for structs
void destroy_data_in_sorted_list(void *);
void insert_data_into_sorted_list(TrieNode *, void *);
int compare_file_nodes(void *, void *);

#define _SEARCH_H 1
#endif
