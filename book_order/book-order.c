#include "book-order.h"

/*
 * 1st arg: Customer database
 * 2nd arg: Book order
 * 3rd arg: Category names
 */
int main(int argc, char *argv[])
{
    Trie *order_trie;
    Trie *customer_trie;

    //Check we have the right number of arguments
    if(argc < 3)
    {
        fprintf(stderr, "Not enough arguments.\n");
        return 1;
    }

    if(argc > 3)
    {
        fprintf(stderr, "Too many arguments.\n");
        return 1;
    }

    //Build prefix tree with category names
    //Queues for consumer threads will be stored here
    order_trie = build_category_trie(argv[3]);

    //Build trie with customer information
    //Output information for each customer will be stored here
    customer_trie = build_customer_trie(argv[1]);

    //Enqueue orders
    enqueue_orders(argv[2]);

    //Now, spin up helper threads to process orders

    //Return results for each customer

    return 0;
}

Trie *build_category_trie(const char *filename)
{
    Trie *t = create_trie(destroy_queue, insert_into_queue);
    FILE *category_file = fopen(filename, "r");
    char category_name[100];

    if(category_file == NULL)
    {
        fprintf(stderr, "Failed to open file %s\n", filename);
        return NULL;
    }

    while(fscanf(category_file, "%s", category_name))
    {
        //FIXME: Insert with new queue
        insert_word(category_name, NULL, t);
        memset(category_name, 0, 100);
    }

    return t;
}
