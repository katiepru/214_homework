#ifndef _INDEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs/trie.h"
#include "structs/sorted-list.h"
#include "structs/ll.h"

//Function declarations
int compareStrings(void *, void *);
int isDelim(char);
void insert_data(TrieNode *, void *);
void destroy_data(TrieNode *, void *);

#define _INDEXER_H 1
#endif
