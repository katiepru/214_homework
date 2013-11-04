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
    int index;

    for(i = 0; i < strlen(word); ++i)
    {
        index = char_to_ind(word[i]);
        if(ptr->children[index] == NULL)
        {
            ptr->children[index] = create_trienode(tolower(word[i]), ptr, 36, t);
        }
        ptr = ptr->children[index];
    }

    t->insert_data(ptr, data);
}


/*
 * converts a char to it's index in the children array.
 */
int char_to_ind(char c)
{
    if (c < 58)
    {
        return c - 48;
    }
    else if (c < 91)
    {
        return c - 55;
    }
    else
    {
        return c - 87;
    }
}


//Functions to allocate and free structs

/*
 * Allocates a Trie
 */
Trie *create_trie(void(*destroy_data)(void *), void(*insert_data)(TrieNode *, void *))
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
    node->data = NULL;

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

    for(i = 0; i < 36; ++i)
    {
        destroy_trienode(node->children[i]);
    }

    node->T->destroy_data(node->data);

    free(node->children);
    free(node);
}

/*
 * performs a dfs and runs a function for each word it finds.
 */
void dfs(TrieNode *root, void(*func)(char*, char*, void*, void*), void *arg, char *file)
{
    char *word;
    int i;

    if (root == NULL)
    {
        return;
    }

    if (root->data != NULL)
    {
        word = get_word(root);
        func(word, file, root->data, arg);
        free(word);
    }

    for (i = 0; i < 36; ++i)
    {
        dfs(root->children[i], func, arg, file);
    }
}

/*
 * returns a char* represented by a given node.
 */
char* get_word(TrieNode *node)
{
    char* word;
    int i;

    if (node == NULL)
    {
        return NULL;
    }

    word = malloc(sizeof(char) * ((node->depth)+1));

    /* terminating null byte */
    word[node->depth] = '\0';
    for (i = (node->depth)-1; i >= 0; --i)
    {
        word[i] = node->c;
        node = node->parent;
    }

    return word;
}
