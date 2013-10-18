#include "fileio.h"

void for_file(char* dirname, void (*func)(), void* arg)
{
    struct dirent *entry;
    char path[1024];
    DIR* dir;

    /* safety check, need non-null pointers */
    if (!dirname || !func || !arg)
    {
        return;
    }

    if (!(dir = opendir(dirname)) || !(entry = readdir(dir)))
    {
        return;
    }

    do
    {
        /* if it's a directory, recurse */
        if (entry->d_type == DT_DIR)
        {
            int len = snprintf(path, sizeof(path)-1, "%s/%s", dirname, entry->d_name);
            path[len] = 0;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }
            for_file(path, func, arg);
        }
        /* if its a regular file, call the function on it */
        else
        {
            func(path, arg);
        }
    } while ((entry = readdir(dir)));
    closedir(dir);
}

void record_file(char *filename, SortedListPtr list)
{
    Tokenizer *tok;
    char *token;

    if (filename == NULL)
    {
        return;
    }

    tok = create_tokenizer(filename, isDelim);

    token = get_next_token(tok);
    while(strlen(token))
    {
        /* replace this with call to add entry to linked list */
        /* record(filename, token); */
        printf("%s: %s\n", filename, token);
        free(token);
        token = get_next_token(tok);
    }

    destroy_tokenizer(tok);
}
