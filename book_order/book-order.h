#ifndef _BOOK_ORDER_H

//Includes
#include <stdio.h>
#include <stdlib.h>

//Local includes
#include "structs/trie.h"

//Function declarations
Trie *build_category_trie(const char *);
Trie *build_customer_trie(const char *);
void enqueue_orders(const char *);

#define _BOOK_ORDER_H 1
#endif
