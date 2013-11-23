#ifndef _BOOK_ORDER_H

//Includes
#include <stdio.h>
#include <stdlib.h>

//Local includes
#include "structs/trie.h"
#include "structs/synch-queue.h"

//Function declarations
Trie *build_category_trie(char **, int, int);
Trie *build_customer_trie(const char *);
void enqueue_orders(const char *);

//Helper functions
void insert_into_queue(TrieNode *, void *);
void destroy_queue(void *);
void destroy_category_queue_node(void *);

#define _BOOK_ORDER_H 1
#endif
