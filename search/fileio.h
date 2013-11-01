#ifndef FILE_IO_H
#define FILE_IO_H
/*
 * fileio.h
 */

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include "tokenizer.h"
#include "structs/sorted-list.h"
#include "structs/file-node.h"
#include "structs/trie.h"

/*
 * for_file takes a directory struct, a pointer to a function, and a pointer to
 * an object that will be passed as the first argument of the function. For each
 * file it finds recursively in the given directory and all its sub-directories,
 * it will call the given function with an open file object and the pointer to
 * the object.
 */
void for_file(char*, void(*)(), void*);


/*
 * write_index takes the final tree and the output location and writes the
 * entire index output.
 */
void write_index(Trie*, char* );

/*
 * write_item is a utility function used by write_index to output indivitual list items.
 */
void write_item(char*, char*, void*, void*);

//Other
void insert_int_data(TrieNode *, void *);
void destroy_int_data(void *);

#endif
