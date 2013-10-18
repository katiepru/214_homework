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

void record_file(char *filename, Trie *main_trie)
{
    Trie *t = create_trie(destroy_int_data, insert_int_data);
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
        insert_word(token, NULL, t);
        free(token);
        token = get_next_token(tok);
    }

    //FIXME: Call something here to do something with the trie
    destroy_tokenizer(tok);
}

void insert_int_data(TrieNode *ptr, void *dummy)
{
    if(ptr->data == NULL)
    {
        ptr->data = malloc(sizeof(int));
        *((int *) ptr->data) = 1;
    }
    *((int *)ptr->data) += 1;
}

void destroy_int_data(void *data)
{
    free(data);
}
