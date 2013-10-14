#ifndef _TRIE_H

#include <stdlib.h>
#include <stdio.h>

//Struct definitions
typedef struct TrieNode TrieNode;
typedef struct Trie Trie;

struct TrieNode {
    char c;
    TrieNode *parent;
    TrieNode **children;
    void *data;
    Trie *T;
};

struct Trie {
    TrieNode *head;
    void (*destroy_data)(void *);
};

//Function declarations
Trie *create_trie(void(*)(void *), TrieNode *);
TrieNode *create_trienode(char, TrieNode *, int, Trie *);
void destroy_trie(Trie *);
void destroy_trienode(TrieNode *);

#define _TRIE_H 1
#endif
