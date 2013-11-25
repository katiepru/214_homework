#ifndef _BOOK_ORDER_H

//Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

//Local includes
#include "structs/trie.h"
#include "structs/synch-queue.h"

//Struct definitions
typedef struct OrderInfo OrderInfo;
struct OrderInfo {
    char *book_name;
    char *category;
    char *cid;
    int price;
    int customer_money_remaining;
};

struct Customer {
    char *name;
    char *id;
    int credit;
    char *address;
    char *state;
    char *zip;
    pthread_mutex_t lock;
    SynchQueue *successful_orders;
    SynchQueue *failed_orders;
};
typedef struct Customer Customer;

typedef struct CategoryArgs CategoryArgs;
struct CategoryArgs {
    TrieNode *category_node;
    Trie *customer_trie;
};

//Function declarations
Trie *build_category_trie(char **, int, int);
Trie *build_customer_trie(const char *);
void enqueue_orders(const char *, Trie *);
void process_orders(Trie *, Trie *, char **, int, int);
void print_results(char *, char *, void *, void *);

//Helper functions
void insert_into_queue(TrieNode *, void *);
void destroy_queue(void *);
void destroy_customer_wrapper(void *);
void *process_category(void *);
void process_order(OrderInfo *, Trie *);

//Struct management fuctions
OrderInfo *create_order(char *, char *, int, char *);
void destroy_order_info(void *);


Customer* create_customer(char*, char*, int, char*, char*, char*);
void destroy_customer(Customer *);


#define _BOOK_ORDER_H 1
#endif
