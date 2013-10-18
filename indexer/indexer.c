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

    //Create data structures
    SortedListPtr list = SLCreate(compareStrings);
    Trie *trie = create_trie(destroy_data, insert_data);
    return 0;
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
void destroy_data(TrieNode *node, void *data)
{
    LL *list = (LL *) data;
    destroy_list(list);
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
