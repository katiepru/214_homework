#include "search.h"

int main(int argc, char **argv)
{
    FILE *input;

    //Checl number of arguments
    if(argc != 2)
    {
        fprintf(stderr, "ERROR: Wrong number of arguments.\n");
        return 1;
    }

    //Open file
    input = fopen(argv[1], "r");

    //Check that it worked
    if(input == NULL)
    {
        fprintf(stderr, "Unable to open file %s.\n", argv[1]);
        return 2;
    }

    Trie *file_contents = preprocess_file(input);

}

/*
 * Preprocess file to make queries fast
 */
Trie *preprocess_file(FILE *input)
{
    int fscanf_result = 1;
    char term[1000];
    char file[1000];
    Trie *trie = create_trie(destroy_data_in_sorted_list, insert_data_into_sorted_list);
    TrieNode *curr_node;
    FileNode *fnode;

    while(fscanf_result != EOF)
    {
        //First, get term
        fscanf_result = fscanf(input, "%s %s", NULL, term);
        if(fscanf_result < 2)
        {
            continue;
        }

        //Next, add the term to the trie
        insert_word(term, NULL, trie);
        curr_node = find_word(term, trie);

        //Term is in trie, now add files
        do {
            fscanf_result = fscanf(input, "%s %d", file, NULL);
            fnode = creat_filenode(file);
            trie->insert_data(curr_node, (void *) fnode);
        } while(fscanf_result > 1);

    }


}

/*
 * Destroys a sorted list
 */
void destroy_data_in_sorted_list(void *data);
{
    SortedListPtr list = (SortedListPtr) data;
    SLDestroy(list);
}

/*
 * Inserts our data into a sorted list
 */
void insert_data_into_sorted_list(TrieNode *node, void *data)
{
    if(node->data == NULL)
    {
        SLCreate(compare_file_nodes);
    }

    SLInsert((SortedListPtr) node->data, data);
}

/*
 * Compares file nodes alphabetically by file name
 */
int compare_file_nodes(void *p1, void *p2)
{
    FileNode *f1 = (FileNode *) p1;
    FileNode *f2 = (FileNode *) p2;

    return strcmp(f1->file_name, f2->file_name);
}
