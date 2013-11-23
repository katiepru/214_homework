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
    order_trie = build_category_trie(argv, 3, argc);

    //Build trie with customer information
    //Output information for each customer will be stored here
    customer_trie = build_customer_trie(argv[1]);

    //Enqueue orders
    enqueue_orders(argv[2], order_trie);

    //Now, spin up helper threads to process orders

    //Return results for each customer

    return 0;
}

Trie *build_category_trie(char **args, int start, int argc)
{
    Trie *t = create_trie(destroy_queue, insert_into_queue);
    int i;
    SynchQueue *q;

    for(i = start; i < argc; i++)
    {
        q = queue_init(destroy_order_info);
        insert_word(args[i], q, t);
    }

    return t;
}

void enqueue_orders(const char *filename, Trie *category_trie)
{
    char *book_name;
    char *book_category;
    int book_price;
    int customer_id;

    OrderInfo *o;
    TrieNode *n;

    char buffer[1024];
    FILE *f = fopen(filename, "r");

    if(f == NULL)
    {
        fprintf(stderr, "Failed to open file %s.\n", filename);
    }

    while(fgets(buffer, sizeof(buffer), f) != NULL)
    {
        book_name = strtok(buffer, "\"|");
        book_price = (int) (100*atol(strtok(NULL, "|")));
        customer_id = atoi(strtok(NULL, "|"));
        book_category = strtok(NULL, "\n");

        //Create order stuct
        o = create_order(book_name, book_category, book_price,
                        customer_id);

        //Enqueue struct into correct queue
        n = find_word(book_category, category_trie);
        category_trie->insert_data(n, (void *)o);
    }
}

//Helpers

void insert_into_queue(TrieNode *n, void *data)
{
    SynchQueue *q = (SynchQueue *) n->data;
    enqueue(q, data);
}

void destroy_queue(void *data)
{
    SynchQueue *q = (SynchQueue *) data;
    queue_destroy(q);
}

//Functions to handle order info structs

OrderInfo *create_order(char *name, char *category,
                       int price, int customer_id)
{
    OrderInfo *o = malloc(sizeof(OrderInfo));

    if(o == NULL)
    {
        fprintf(stderr, "Malloc failed\n");
        return NULL;
    }

    o->book_name = malloc(strlen(name) + 1);
    strcpy(o->book_name, name);

    o->category = malloc(strlen(category) + 1);
    strcpy(o->category, category);

    o->price = price;
    o->cid = customer_id;

    return o;

}

void destroy_order_info(void *o)
{
    if(o == NULL) return;

    OrderInfo *order = (OrderInfo *) o;

    free(order->book_name);
    free(order->category);

    free(order);
}
