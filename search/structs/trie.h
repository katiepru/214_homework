#ifndef _TRIE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//Struct definitions
typedef struct TrieNode TrieNode;
typedef struct Trie Trie;

struct TrieNode {
    char c;
    TrieNode *parent;
    TrieNode **children;
    void *data;
    Trie *T;
    int depth;
};

struct Trie {
    TrieNode *head;
    void (*destroy_data)(void *);
    void (*insert_data)(TrieNode *, void *);
};

//Function declarations
Trie *create_trie(void(*)(void *), void(*)(TrieNode *, void *));
int char_to_ind(char);
TrieNode *create_trienode(char, TrieNode *, int, Trie *);
TrieNode *find_word(char *, Trie *);
void insert_word(char *, void *, Trie *);
void destroy_trie(Trie *);
void destroy_trienode(TrieNode *);
void dfs(TrieNode *, void(*)(char *, char*, void *, void *), void*, char*);
char* get_word(TrieNode*);

#define _TRIE_H 1
#endif
