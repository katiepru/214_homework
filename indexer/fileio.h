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
void for_file(char*, void*, void*);

#endif
