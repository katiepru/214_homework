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
    dfs(trie->head, write_item, argv[1], NULL);
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
 * Inserts a file into the list
 * Data is filename
 */
void insert_data(TrieNode *node, void *data)
{
    LL *list = (LL *) node->data;
    char *filename = (char *) data;
    LLNode *n;

    if(strcmp(list->curr->file_name, filename) != 0)
    {
        n = create_llnode(filename, list);
        insert_node(list, n);
    }
    else
    {
        list->curr->count++;
    }
}

/*
 * Destroys linked list from trienode
 */
void destroy_data(void *data)
{
    LL *list = (LL *) data;
    destroy_list(list);
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

/*
 * String compare function
 * Works backwards to get list to work with ascending order
 * Args are TrieNodes
 */
int compareStrings(void *p1, void *p2)
{
    TrieNode *ptr1 = p1;
    TrieNode *ptr2 = p2;

    char s1[ptr2->depth + 1];
    char s2[ptr1->depth + 1];

    s2[ptr1->depth + 1] = 0;
    s1[ptr2->depth + 1] = 0;

    int i;

    //Get word from p1
    i = ptr1->depth;
    while(ptr1->parent != NULL)
    {
        s2[i] = ptr1->c;
        ptr1 = ptr1->parent;
        i--;
    }

    i = ptr2->depth;
    while(ptr2->parent != NULL)
    {
        s1[i] = ptr2->c;
        ptr2 = ptr2->parent;
        i--;
    }

    return strcmp(s1, s2);
}

/*
 * Is delim function
 */
int isDelim(char c)
{
    return (c < 48 || (c > 57 && c < 65) || (c > 90 && c < 97) || c > 122);
}
