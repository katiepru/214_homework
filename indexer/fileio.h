#ifndef SORTED_LIST_H
#define SORTED_LIST_H
/*
 * fileio.h
 */

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

/*
 * for_file takes a directory struct, a pointer to a function, and a pointer to
 * an object that will be passed as the first argument of the function. For each
 * file it finds recursively in the given directory and all its sub-directories,
 * it will call the given function with an open file object and the pointer to
 * the object.
 */
void for_file(char*, void(*)(), void*);

/*
 * record_file takes the path to a file, tokenizes it, and then calls a given
 * function once for each token with the file path and the token as char*-s.
 */
void record_file(char*, void(*)());

#endif
