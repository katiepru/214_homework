#include "indexer.h"

int main(int argc, char *argv[])
{
    //Check that we have the right number of args
    if(argc < 3)
    {
        fprintf(stderr, "Please provide the output file and directory to index\n");
        return 1;
    }
    if(argc > 3)
    {
        fprintf(stderr, "Please only provide the output file and directory to index\n");
        return 1;
    }

    //Create the master trie
    Trie *trie = create_trie(destroy_data_sorted_list, insert_data_sorted_list);
    for_file(argv[2], record_file, trie);
    write_index(trie, argv[1]);

    destroy_trie(trie);
    return 0;
}

/*
 * Inserts word into master trie
 */
void insert_into_master(char *word, char *filename, void *data, void *arg)
{
    Trie *master = (Trie *) arg;
    int count = *((int *) data);
    FileNode *f = create_filenode(filename);
    f->count = count;

    insert_word(word, (void *) f, master);
}

/*
 * Inserts file info node into sorted list
 */
void insert_data_sorted_list(TrieNode *node, void *data)
{
    SortedListPtr list = (SortedListPtr) node->data;

    if(list == NULL)
    {
        list = SLCreate(compare_file_nodes);
    }

    SLInsert(list, data);
}

/*
 * Destroys a sorted list
 */
void destroy_data_sorted_list(void *data)
{
    SortedListPtr list = (SortedListPtr) data;
    SLDestroy(list);
}

/*
 * Compare two file nodes
 */
int compare_file_nodes(void *p1, void *p2)
{
    FileNode *f1 = (FileNode *) p1;
    FileNode *f2 = (FileNode *) p2;

    return f1->count - f2->count;
}

void record_file(char *filename, Trie *main_trie)
{
    Trie *t = create_trie(destroy_int_data, insert_int_data);
    Tokenizer *tok;
    char *token;

    if (filename == NULL)
    {
        return;
    }

    tok = create_tokenizer(filename, isDelim);

    token = get_next_token(tok);
    while(token != NULL && strlen(token))
    {
        insert_word(token, NULL, t);
        free(token);
        token = get_next_token(tok);
    }

    dfs(t->head, insert_into_master, main_trie, filename);
    destroy_trie(t);
    destroy_tokenizer(tok);
}
