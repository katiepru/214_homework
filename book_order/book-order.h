#ifndef _BOOK_ORDER_H

//Includes
#include <stdio.h>
#include <stdlib.h>

//Local includes
#include "structs/trie.h"
#include "structs/synch-queue.h"

//Struct definitions
typedef struct OrderInfo OrderInfo;
struct OrderInfo {
    char *book_name;
    char *category;
    int cid;
    int price;
};

//Function declarations
Trie *build_category_trie(char **, int, int);
Trie *build_customer_trie(const char *);
void enqueue_orders(const char *, Trie *);

//Helper functions
void insert_into_queue(TrieNode *, void *);
void destroy_queue(void *);

//Struct management fuctions
OrderInfo *create_order(char *, char *, int, int);
void destroy_order_info(void *);


#define _BOOK_ORDER_H 1
#endif
