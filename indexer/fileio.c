#include "fileio.h"

void for_file(char* dirname, void* func, void* arg)
{
    struct dirent *entry;
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
        if (entry->d_type == DT_DIR)
        {
            char path[1024];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", dirname, entry->d_name);
            path[len] = 0;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            /* recursive call here */
        }
        else
        {
            /* call function here */
        }
    } while ((entry = readdir(dir)));
    closedir(dir);
}
