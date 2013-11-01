#ifndef _INDEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileio.h"
#include "tokenizer.h"
#include "structs/trie.h"
#include "structs/sorted-list.h"
#include "structs/ll.h"
#include "structs/file-node.h"

//Function declarations
int compare_file_nodes(void *, void *);
int compareStrings(void *, void *);
void insert_into_master(char *, char *, void *, void *);
void insert_data_sorted_list(TrieNode *, void *);
void insert_data(TrieNode *, void *);
void destroy_data_sorted_list(void *);
void destroy_data(void *);

/*
 * record_file takes the path to a file, tokenizes it, and then calls a given
 * function once for each token with the file path and the token as char*-s.
 */
void record_file(char*,Trie *);

#define _INDEXER_H 1
#endif
