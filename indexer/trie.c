#include "trie.h"

/*
 * Allocates a Trie
 */
Trie *create_trie(void(*destroy_data)(void *), TrieNode *head)
{
    Trie *t = malloc(sizeof(Trie));

    if(t == NULL)
    {
        fprintf(stderr, "Malloc failed.\n");
        exit(1);
    }

    t->head = head;
    t->destroy_data = destroy_data;

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
