#include "trie.h"

//Functions to use Tries

/*
 * Returns TrieNode that is the end of the word
 * Assumes Trie arg is not NULL and word is lower case
 */
TrieNode *find_word(char *word, Trie *t)
{
    int i;
    TrieNode *ptr = t->head;

    for(i = 0; i < strlen(word); ++i)
    {
        if(ptr == NULL)
        {
            return NULL;
        }

        ptr = ptr->children[char_to_ind(word[i])];
    }

    return ptr;
}

/*
 *
 */
void insert_word(char *word, void *data, Trie *t)
{
    int i;
    TrieNode *ptr = t->head;
    TrieNode *tmp;

    for(i = 0; i < strlen(word); ++i)
    {
        if((tmp = ptr->children[char_to_ind(word[i])]) == NULL)
        {
            //FIXME: num
            tmp = create_trienode(word[i], ptr, 0, t);
        }
        ptr = tmp;
    }

    t->insert_data(ptr, data);
}


//Functions to allocate and free structs

/*
 * Allocates a Trie
 */
Trie *create_trie(void(*destroy_data)(TrieNode *, void *), void(*insert_data)(TrieNode *, void *))
{
    Trie *t = malloc(sizeof(Trie));

    if(t == NULL)
    {
        fprintf(stderr, "Malloc failed.\n");
        exit(1);
    }

    t->head = create_trienode('\0', NULL, 36, t);
    t->destroy_data = destroy_data;
    t->insert_data = insert_data;

    return t;
}

/*
 * Frees a Trie
 */
void destroy_trie(Trie *t)
{
    if(t == NULL)
    {
        return;
    }

    destroy_trienode(t->head);

    free(t);
}

/*
 * Allocates a trienode
 */
TrieNode *create_trienode(char c, TrieNode *parent, int num_children, Trie *T)
{
    TrieNode *node = malloc(sizeof(TrieNode));

    if(node == NULL)
    {
        fprintf(stderr, "Malloc failed.\n");
        exit(1);
    }

    if(parent != NULL)
    {
        node->depth = parent->depth + 1;
    }
    else
    {
        node->depth = 0;
    }

    node->c = c;
    node->parent = parent;
    node->children = calloc(num_children, sizeof(TrieNode *));
    node->T = T;

    return node;
}

/*
 * Frees a trienode
 */
void destroy_trienode(TrieNode *node)
{
    int i;

    if(node == NULL)
    {
        return;
    }

    for(i = 0; i < (sizeof(node->children)/sizeof(TrieNode *)); ++i)
    {
        destroy_trienode(node->children[i]);
    }

    node->T->destroy_data(node->data);

    free(node->children);
}
