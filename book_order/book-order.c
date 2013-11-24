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

    //Build prefix tree with category names
    //Queues for consumer threads will be stored here
    order_trie = build_category_trie(argv, 3, argc);

    //Build trie with customer information
    //Output information for each customer will be stored here
    customer_trie = build_customer_trie(argv[1]);

    //Enqueue orders
    enqueue_orders(argv[2], order_trie);

    //Now, spin up helper threads to process orders
    //FIXME: Single threaded solution
    process_orders(order_trie, customer_trie, argv, 3, argc);

    //Return results for each customer
    dfs(customer_trie->head, print_results, NULL, NULL);

    // Free all the things!
    destroy_trie(customer_trie);
    destroy_trie(order_trie);

    return 0;
}

void process_orders(Trie *order_trie, Trie *customer_trie,
                    char **args, int start, int argc)
{
    TrieNode *curr;
    pthread_t threads[argc-start];
    CategoryArgs *cargs[argc-start];
    int i;

    //For each category
    for(i = start; i < argc; i++)
    {
        //Find entry in order trie
        curr = find_word(args[i], order_trie);

        if(curr == NULL || curr->data == NULL)
        {
            fprintf(stderr, "Could not find orders for %s\n",
                    args[i]);
            continue;
        }

        cargs[i-start] = malloc(sizeof(CategoryArgs));
        if(cargs == NULL)
        {
            fprintf(stderr, "Malloc failed.\n");
            return;
        }

        cargs[i-start]->category_node = curr;
        cargs[i-start]->customer_trie = customer_trie;

        pthread_create(&(threads[i-start]), NULL,
                       process_category, (void *) cargs[i-start]);
    }

    for(i = 0; i < (argc-start); i++)
    {
        pthread_join(threads[i], NULL);
        free(cargs[i]);
    }
}

void *process_category(void *args)
{
    OrderInfo *order;
    SynchQueue *category_q;
    CategoryArgs *c = (CategoryArgs *) args;

    //Get it's queue
    category_q = (SynchQueue *) c->category_node->data;

    //Go through and process each order
    while((order = dequeue(category_q)) != NULL)
    {
        process_order(order, c->customer_trie);
    }
    return NULL;
}

void process_order(OrderInfo *o, Trie *customer_trie)
{
    Customer *customer;
    TrieNode *found;

    if(o == NULL) return;

    found = find_word(o->cid, customer_trie);

    if(!found || found->data == NULL)
    {
        fprintf(stderr, "Could not find customer %s.\n", o->cid);
        return;
    }

    customer = (Customer *) found->data;

    pthread_mutex_lock(&(customer->lock));

    if(!customer->successful_orders) {
        customer->successful_orders = queue_init(destroy_customer_wrapper);
        customer->failed_orders = queue_init(destroy_customer_wrapper);
    }

    //Cannot afford this book
    if(customer->credit < o->price)
    {
        enqueue(customer->failed_orders, (void *) o);
    }
    else
    {
        enqueue(customer->successful_orders, (void *) o);
        customer->credit -= o->price;
    }

    pthread_mutex_unlock(&(customer->lock));
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
    char *customer_id;

    OrderInfo *o;
    TrieNode *n;
    SynchQueue *q;

    char buffer[1024];
    FILE *f = fopen(filename, "r");

    if(f == NULL)
    {
        fprintf(stderr, "Failed to open file %s.\n", filename);
    }

    while(fgets(buffer, sizeof(buffer), f) != NULL)
    {
        book_name = strtok(buffer, "\"|");
        book_price = (int) (100*atof(strtok(NULL, "|")));
        customer_id = strtok(NULL, "|");
        book_category = strtok(NULL, "\n");

        //Create order stuct
        o = create_order(book_name, book_category, book_price,
                        customer_id);

        //Enqueue struct into correct queue
        n = find_word(book_category, category_trie);
        q = (SynchQueue *) n->data;
        enqueue(q, (void *) o);
    }

    fclose(f);
}

void print_results(char *cid, char *dummy, void *data,
                   void *dummy2)
{
    Customer *customer = (Customer *) data;
    OrderInfo *curr;

    printf("%s - %s\n", customer->name, customer->id);
    printf("Successful orders:\n");
    if(!customer->successful_orders)
    {
        printf("\tNone\n");
    }
    else
    {
        while((curr = dequeue(customer->successful_orders)) != NULL)
        {
            printf("\tTitle: %s\n\tPrice: %d\n", curr->book_name,
                   curr->price);
            destroy_order_info(curr);
        }
        queue_destroy(customer->successful_orders);
    }

    printf("\nUnsuccessful orders:\n");
    if(!customer->failed_orders)
    {
        printf("\tNone\n");
    }
    else
    {
        while((curr = dequeue(customer->failed_orders)) != NULL)
        {
            printf("\tTitle: %s\n\tPrice: %d\n", curr->book_name,
                   curr->price);
            destroy_order_info(curr);
        }
        queue_destroy(customer->failed_orders);
    }
}

//Helpers

void insert_into_queue(TrieNode *n, void *data)
{
    if(n->data == NULL)
    {
        n->data = data;
    }
}

void destroy_queue(void *data)
{
    SynchQueue *q = (SynchQueue *) data;
    if(q == NULL) return;
    queue_destroy(q);
}

void destroy_customer_wrapper(void *data)
{
    Customer *c = (Customer *) data;
    destroy_customer(c);
}

//Functions to handle order info structs

OrderInfo *create_order(char *name, char *category,
                       int price, char *customer_id)
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

    o->cid = malloc(strlen(customer_id) + 1);
    strcpy(o->cid, customer_id);

    o->price = price;

    return o;

}

void destroy_order_info(void *o)
{
    if(o == NULL) return;

    OrderInfo *order = (OrderInfo *) o;

    free(order->book_name);
    free(order->category);
    free(order->cid);

    free(order);
}

Trie *build_customer_trie(const char *filename) {
    Trie *t;
    FILE *customer_file;
    char line[1024];
    char *customer_name;
    char *customer_id;
    int customer_credit;
    char *customer_address;
    char *customer_state;
    char *customer_zip;
    Customer *new_customer;


    customer_file = fopen(filename, "r");
    if (customer_file == NULL) {
        fprintf(stderr, "Failed to open file %s\n", filename);
        return NULL;
    }

    t = create_trie(destroy_customer_wrapper, insert_into_queue);

    while (fgets(line, sizeof(line), customer_file) != NULL) {
        customer_name = strtok(&(line[1]), "\"|");
        customer_id = strtok(NULL, "|");
        customer_credit = (int) (strtof(strtok(NULL, "|\""), NULL) * 100);
        customer_address = strtok(NULL, "\"|\"");
        customer_state = strtok(NULL, "\"|\"");
        customer_zip = strtok(NULL, "\"");

        new_customer = create_customer(customer_name, customer_id, customer_credit, customer_address, customer_state, customer_zip);

        insert_word(customer_id, new_customer, t);
    }

    fclose(customer_file);

    return t;
}


Customer* create_customer(char* name, char* id, int credit, char* address, char* state, char* zip)
{
    Customer *new_customer = malloc(sizeof(Customer));

    new_customer->name  = strcpy(malloc(strlen(name) + 1), name);
    new_customer->id  = strcpy(malloc(strlen(id) + 1), id);
    new_customer->credit = credit;
    new_customer->address = strcpy(malloc(strlen(address) + 1), address);
    new_customer->state = strcpy(malloc(strlen(state) + 1), state);
    new_customer->zip = strcpy(malloc(strlen(zip) + 1), zip);
    new_customer->failed_orders = NULL;
    new_customer->successful_orders = NULL;

    pthread_mutex_init(&new_customer->lock, NULL);

    return new_customer;
}

void destroy_customer(Customer *cust)
{
    if (cust == NULL) {
        return;
    }

    free(cust->name);
    free(cust->id);
    free(cust->address);
    free(cust->state);
    free(cust->zip);

    pthread_mutex_destroy(&(cust->lock));

    free(cust);
}
