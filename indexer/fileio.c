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
        int len = snprintf(path, sizeof(path)-1, "%s/%s", dirname, entry->d_name);
        /* if it's a directory, recurse */
        if (entry->d_type == DT_DIR)
        {
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

void insert_int_data(TrieNode *ptr, void *dummy)
{
    if(ptr->data == NULL)
    {
        ptr->data = malloc(sizeof(int));
        *((int *) ptr->data) = 1;
    }
    else
    {
        *((int *)ptr->data) += 1;
    }
}

void destroy_int_data(void *data)
{
    free(data);
}

void write_index(Trie* tree, char* location)
{
    FILE *output;

    if (tree == NULL || location == NULL)
    {
        return;
    }

    output = fopen(location, "ab+");
    if (output == NULL) return;

    dfs(tree->head, write_item, output, NULL);

    fclose(output);
}

void write_item(char *word, char *unused, void* list, void* output)
{
    SortedListIteratorPtr iter;
    FileNode *info;
    int entries = 0;

    iter = SLCreateIterator(list);
    info = SLNextItem(iter);

    fprintf(output, "<list> %s\n", word);
    while(info != NULL) {
        if (entries > 0 && entries % 5 == 0) fprintf(output, "\n");
        fprintf(output, "%s %d ", info->file_name, info->count);
        entries++;
        destroy_filenode(info);  /* FIXME: move this functionality into the SL */
        info = SLNextItem(iter);
    }
    fprintf(output, "\n</list>\n");

    SLDestroyIterator(iter);
}
