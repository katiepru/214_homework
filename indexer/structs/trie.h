#ifndef _TRIE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    void (*destroy_data)(TrieNode *, void *);
    void (*insert_data)(TrieNode *, void *);
};

//Function declarations
char char_to_ind(char);
Trie *create_trie(void(*)(TrieNode *, void *), void(*)(TrieNode *, void *), TrieNode *);
TrieNode *create_trienode(char, TrieNode *, int, Trie *);
TrieNode *find_word(char *, Trie *);
void insert_word(char *, void *, Trie *);
void destroy_trie(Trie *);
void destroy_trienode(TrieNode *);

#define _TRIE_H 1
#endif
