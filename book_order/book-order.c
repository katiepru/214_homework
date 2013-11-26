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
    int total_revenue = 0;
    pthread_t producer;
    EnqueueOrdersArgs *enqueue_orders_args;

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

    //Enqueue orders - make args struct
    enqueue_orders_args = malloc(sizeof(EnqueueOrdersArgs));
    if(enqueue_orders_args == NULL)
    {
        fprintf(stderr, "Malloc failed.\n");
        return 2;
    }
    enqueue_orders_args->filename = argv[2];
    enqueue_orders_args->order_trie = order_trie;

    //Spawn producer to enqueue orders
    pthread_create(&producer, NULL, enqueue_orders, (void *) enqueue_orders_args);

    //Now, spin up helper threads to process orders
    process_orders(order_trie, customer_trie, argv, 3, argc);

    //Take care of producer
    pthread_join(producer, NULL);

    //Return results for each customer
    printf("\n\n~~~~~~FINAL REPORT~~~~~~\n");
    dfs(customer_trie->head, print_results, (void*) &total_revenue, NULL);

    printf("Total revenue: %.2f\n", (double) total_revenue / 100);

    // Free all the things!
    destroy_trie(customer_trie);
    destroy_trie(order_trie);
    free(enqueue_orders_args);

    return 0;
}

/*
 * Spawns one consumer thread per category
 * These consumer threads call process_category
 */
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

/*
 * Run by each consumer thread
 * While we have not reach the end of the orders for this category,
 * use a semaphore to wait until something in enqueued
 * dequeue and process the order
 * Returns NULL always
 */
void *process_category(void *args)
{
    OrderInfo *order;
    SynchQueue *category_q;
    CategoryArgs *c = (CategoryArgs *) args;

    //Get it's queue
    category_q = (SynchQueue *) c->category_node->data;

    sem_wait(&(category_q->semaphore));
    //Go through and process each order
    while((order = dequeue(category_q)) != NULL)
    {
        process_order(order, c->customer_trie);
        sem_wait(&(category_q->semaphore));
    }
    return NULL;
}

/*
 * Thread locks customer object and processes the current order
 * Unlocks once it is done modifying customer struct
 */
void process_order(OrderInfo *o, Trie *customer_trie)
{
    Customer *customer;
    TrieNode *found;

    if(o == NULL) return;

    found = find_word(o->cid, customer_trie);

    if(!found || found->data == NULL)
    {
        fprintf(stderr, "BAD INPUT: COULD NOT FIND CUSTOMER WITH ID %s\n", o->cid);
        exit(1);
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
        printf("=== REJECTED ORDER ===\n"
        "Customer:\n\t%s\n"
        "Available Credit:\n\t%.2f\n"
        "Book:\n\t%s\n"
        "\t%.2f\n",
        customer->name,
        (double) customer->credit / 100,
        o->book_name,
        (double) o->price / 100);

        enqueue(customer->failed_orders, (void *) o);
    }
    else
    {
        o->customer_money_remaining = customer->credit - o->price;
        enqueue(customer->successful_orders, (void *) o);
        customer->credit -= o->price;

        printf("=== SUCCESSFUL ORDER ===\n"
        "Book:\n"
        "\t%s\n"
        "\t%.2f\n"
        "Ship To:\n"
        "\t%s\n"
        "\t%s\n"
        "\t%s\n"
        "\t%s\n",
        o->book_name,
        (double) o->price / 100,
        customer->name,
        customer->address,
        customer->state,
        customer->zip);
    }

    pthread_mutex_unlock(&(customer->lock));
}

/*
 * Reads through category names and builds prefix tree
 * Each end node will have a pointer to a queue of orders
 * for that category
 */
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

/*
 * Builds the customer trie from customer database file
 * Uses customer ids as words in trie
 */
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
        exit(2);
    }

    t = create_trie(destroy_customer_wrapper, insert_into_queue);

    while (fgets(line, sizeof(line), customer_file) != NULL) {
        customer_name = strtok(&(line[1]), "\"|");
        customer_id = strtok(NULL, "|");
        customer_credit = (int) (strtof(strtok(NULL, "|\""), NULL) * 100);
        customer_address = strtok(NULL, "\"|\"");
        customer_state = strtok(NULL, "\"|\"");
        customer_zip = strtok(NULL, "|\"");

        new_customer = create_customer(customer_name, customer_id, customer_credit, customer_address, customer_state, customer_zip);

        insert_word(customer_id, new_customer, t);
    }

    fclose(customer_file);

    return t;
}

/*
 * Producer thread reads in orders file
 * An OrderInfo struct is created and enqueued into the appropriate
 * category queue in the order trie
 * Once something is enqueued, post to the semaphore
 * Add a NULL OrderInfo struct as the last item in the queue to signal
 * that we are done once we see it
 */
void *enqueue_orders(void *args)
{
    char *book_name;
    char *book_category;
    int book_price;
    char *customer_id;

    OrderInfo *o;
    TrieNode *n;
    SynchQueue *q;

    Trie *order_trie = ((EnqueueOrdersArgs *) args)->order_trie;
    const char *filename = ((EnqueueOrdersArgs *) args)->filename;

    char buffer[1024];
    FILE *f = fopen(filename, "r");

    void *end = NULL;

    if(f == NULL)
    {
        fprintf(stderr, "Failed to open file %s.\n", filename);
        exit(2);
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
        n = find_word(book_category, order_trie);
        if (n == NULL || n->data == NULL) {
            fprintf(stderr, "BAD INPUT: INVALID CATEGORY: %s\n", book_category);
            exit(1);
        }
        q = (SynchQueue *) n->data;
        enqueue(q, (void *) o);
        sem_post(&(q->semaphore));
    }

    dfs(order_trie->head, insert_null, end, NULL);

    fclose(f);

    return NULL;
}

/*
 * Print out results from customer trie
 * Calculates total revenue as well
 */
void print_results(char *cid, char *dummy, void *data,
                   void *total_revenue)
{
    Customer *customer = (Customer *) data;
    OrderInfo *curr;

    printf("=== BEGIN CUSTOMER INFO ===\n### BALANCE ###\n");
    printf("Customer name: %s\n", customer->name);
    printf("Customer ID number: %s\n", customer->id);
    printf("Remaining credit balance after all purchases (a dollar amount): %.2f\n", (double) customer->credit / 100);
    printf("### SUCCESSFUL ORDERS ###\n");

    while((curr = dequeue(customer->successful_orders)) != NULL)
    {
        printf("\"%s\"| %.2f| %.2f\n",
                curr->book_name,
                (double) curr->price / 100,
                (double) curr->customer_money_remaining / 100);
        *((int*)total_revenue) += curr->price;
        destroy_order_info(curr);
    }
    queue_destroy(customer->successful_orders);

    printf("### REJECTED ORDERS ###\n");
    while((curr = dequeue(customer->failed_orders)) != NULL)
    {
        printf("\"%s\"| %.2f\n",
                curr->book_name,
                (double) curr->price / 100);
        destroy_order_info(curr);
    }
    queue_destroy(customer->failed_orders);

    printf("=== END CUSTOMER INFO ===\n\n");
}

//Helpers

/*
 * Wrapper to set data in trie
 */
void insert_into_queue(TrieNode *n, void *data)
{
    if(n->data == NULL)
    {
        n->data = data;
    }
}

/*
 * Wrapper to destroy queue in trie
 */
void destroy_queue(void *data)
{
    SynchQueue *q = (SynchQueue *) data;
    if(q == NULL) return;
    queue_destroy(q);
}

/*
 * Wrapper to destroy customer in trie
 */
void destroy_customer_wrapper(void *data)
{
    Customer *c = (Customer *) data;
    destroy_customer(c);
}

/*
 * Inserts NULL into each queue in the orders trie
 */
void insert_null(char *dummy, char *dummy2, void *queue, void *null_ptr)
{
    SynchQueue *order_queue = (SynchQueue *) queue;
    enqueue(order_queue, null_ptr);
    sem_post(&(order_queue->semaphore));
}

//Functions to handle order info structs

/*
 * Malloc and initialize OrderInfo struct
 */
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
    o->customer_money_remaining = -1;

    return o;

}

/*
 * Free OrderInfo struct
 */
void destroy_order_info(void *o)
{
    if(o == NULL) return;

    OrderInfo *order = (OrderInfo *) o;

    free(order->book_name);
    free(order->category);
    free(order->cid);

    free(order);
}

/*
 * Malloc and initialize Customer struct
 */
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

/*
 * Free customer struct
 */
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
